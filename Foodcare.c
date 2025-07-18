/**
 * @file foodcare.c
 * @brief Proyecto FoodCare: Sistema embebido para el control automatizado
 *        del entorno en una zona de manipulación de alimentos.
 *
 * Este programa implementa sensores de temperatura, humedad, movimiento y
 * presencia de productos, además de controlar motores para ventilar y accionar
 * mecanismos automáticos, incluyendo botones de encendido/apagado.
 *
 * @author Sebastian Balbin Rivera
 * @date 2025-07-17
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/time.h"

// --- Definiciones de pines ---
#define DHT11_GPIO     15     ///< GPIO para sensor DHT11
#define SENSOR1_GPIO   14     ///< Sensor IR para entrada de productos
#define SENSOR2_GPIO   13     ///< Sensor IR para salida de productos
#define PIR_GPIO       12     ///< Sensor de movimiento PIR
#define LED_GPIO       11     ///< LED indicador de movimiento

#define IN1 9                ///< Motor 1 dirección IN1
#define IN2 8                ///< Motor 1 dirección IN2
#define ENA 10               ///< Motor 1 PWM

#define IN1_2 6             ///< Motor 2 dirección IN1
#define IN2_2 7             ///< Motor 2 dirección IN2
#define ENA2 5              ///< Motor 2 PWM

#define BTN_START 18        ///< Botón para encender motor 2
#define BTN_STOP  19        ///< Botón para apagar motor 2

// --- Variables globales ---
volatile bool sensor1_triggered = false;
volatile bool sensor2_triggered = false;
volatile bool motion_detected = false;
volatile bool motor2_on = false;
bool led_on = false;
int inventario = 0; ///< Contador de productos en inventario
absolute_time_t last_motion_time;

/**
 * @brief Configura un pin GPIO como salida PWM con un cierto porcentaje.
 * @param gpio Pin PWM a configurar.
 * @param porcentaje Valor de 0.0 a 1.0 indicando el duty cycle.
 */
void setup_pwm(uint gpio, float porcentaje);

/**
 * @brief Detiene la salida PWM del pin especificado.
 * @param gpio Pin PWM que se desea apagar.
 */
void stop_pwm(uint gpio);

/**
 * @brief Inicializa el sensor DHT11.
 */
void DHT11_init();

/**
 * @brief Lee los datos del sensor DHT11.
 * @param temperature Puntero donde se guarda la temperatura leída.
 * @param humidity Puntero donde se guarda la humedad leída.
 * @return 0 si la lectura fue exitosa, negativo si hubo error.
 */
int DHT11_read(int *temperature, int *humidity);

/**
 * @brief Configura un sensor IR para detección por interrupciones.
 * @param gpio Pin donde está conectado el sensor IR.
 */
void init_ir_sensor(uint gpio);

/**
 * @brief Inicializa el sensor de movimiento PIR.
 */
void init_pir_sensor();

/**
 * @brief Inicializa el LED indicador.
 */
void init_led();

/**
 * @brief Inicializa los botones de control.
 */
void init_buttons();

/**
 * @brief Manejador de interrupciones para sensores IR y PIR.
 * @param gpio GPIO que generó la interrupción.
 * @param events Tipo de evento (subida/bajada).
 */
void sensor_irq_handler(uint gpio, uint32_t events);

// Implementación de funciones

void setup_pwm(uint gpio, float porcentaje) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);
    uint32_t sys_clk = 125000000;
    uint32_t divider16 = sys_clk * 16 / 150 / 65536 + 1;
    if (divider16 < 16) divider16 = 16;
    uint32_t top = sys_clk * 16 / divider16 / 150;
    pwm_set_clkdiv_int_frac(slice, divider16 / 16, divider16 & 0xF);
    pwm_set_wrap(slice, top);
    pwm_set_chan_level(slice, pwm_gpio_to_channel(gpio), top * porcentaje);
    pwm_set_enabled(slice, true);
}

void stop_pwm(uint gpio) {
    uint slice = pwm_gpio_to_slice_num(gpio);
    pwm_set_chan_level(slice, pwm_gpio_to_channel(gpio), 0);
}

void DHT11_init() {
    gpio_init(DHT11_GPIO);
    gpio_set_dir(DHT11_GPIO, GPIO_OUT);
    gpio_put(DHT11_GPIO, 1);
}

static void set_input() { gpio_set_dir(DHT11_GPIO, GPIO_IN); }
static void set_output() { gpio_set_dir(DHT11_GPIO, GPIO_OUT); }

int DHT11_read(int *temperature, int *humidity) {
    uint8_t data[5] = {0};
    set_output(); gpio_put(DHT11_GPIO, 0); sleep_ms(18);
    gpio_put(DHT11_GPIO, 1); busy_wait_us(30); set_input();

    if (gpio_get(DHT11_GPIO)) return -1;
    while (!gpio_get(DHT11_GPIO)); while (gpio_get(DHT11_GPIO));

    for (int i = 0; i < 40; i++) {
        while (!gpio_get(DHT11_GPIO));
        busy_wait_us(28);
        if (gpio_get(DHT11_GPIO)) data[i / 8] |= (1 << (7 - (i % 8)));
        while (gpio_get(DHT11_GPIO));
    }

    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (data[4] != checksum) return -2;
    *humidity = data[0]; *temperature = data[2];
    return 0;
}

void sensor_irq_handler(uint gpio, uint32_t events) {
    if ((events & GPIO_IRQ_EDGE_FALL)) {
        if (gpio == SENSOR1_GPIO) { sensor1_triggered = true; }
        else if (gpio == SENSOR2_GPIO) { sensor2_triggered = true; }
    } else if ((events & GPIO_IRQ_EDGE_RISE)) {
        if (gpio == PIR_GPIO) motion_detected = true;
    }
}

void init_ir_sensor(uint gpio) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
    gpio_set_irq_enabled_with_callback(gpio, GPIO_IRQ_EDGE_FALL, true, &sensor_irq_handler);
}

void init_pir_sensor() {
    gpio_init(PIR_GPIO);
    gpio_set_dir(PIR_GPIO, GPIO_IN);
    gpio_pull_down(PIR_GPIO);
    gpio_set_irq_enabled_with_callback(PIR_GPIO, GPIO_IRQ_EDGE_RISE, true, &sensor_irq_handler);
}

void init_led() {
    gpio_init(LED_GPIO);
    gpio_set_dir(LED_GPIO, GPIO_OUT);
    gpio_put(LED_GPIO, 0);
}

void init_buttons() {
    gpio_init(BTN_START); gpio_set_dir(BTN_START, GPIO_IN); gpio_pull_down(BTN_START);
    gpio_init(BTN_STOP);  gpio_set_dir(BTN_STOP, GPIO_IN);  gpio_pull_down(BTN_STOP);
}

/**
 * @brief Programa principal.
 *
 * Inicializa sensores, motores, LED y botones.
 * Lee temperatura y humedad cada segundo.
 * Activa motor 1 si la humedad supera 60% o la temperatura supera 30°C.
 * Controla LED por movimiento PIR.
 * Controla motor 2 con botones.
 * Cuenta productos que entran y salen mediante sensores IR.
 */
int main() {
    stdio_init_all();

    DHT11_init();
    init_ir_sensor(SENSOR1_GPIO);
    init_ir_sensor(SENSOR2_GPIO);
    init_pir_sensor();
    init_led();
    init_buttons();

    // Motor 1
    gpio_init(IN1); gpio_set_dir(IN1, GPIO_OUT);
    gpio_init(IN2); gpio_set_dir(IN2, GPIO_OUT);
    gpio_put(IN1, 1); gpio_put(IN2, 0);
    setup_pwm(ENA, 0);

    // Motor 2
    gpio_init(IN1_2); gpio_set_dir(IN1_2, GPIO_OUT);
    gpio_init(IN2_2); gpio_set_dir(IN2_2, GPIO_OUT);
    gpio_put(IN1_2, 1); gpio_put(IN2_2, 0);
    setup_pwm(ENA2, 0);

    absolute_time_t last_dht11_read = get_absolute_time();

    while (true) {
        if (absolute_time_diff_us(last_dht11_read, get_absolute_time()) >= 1000000) {
            int temp = 0, hum = 0;
            if (DHT11_read(&temp, &hum) == 0) {
                printf("🌡️ Temp: %d°C  💧 Humedad: %d%%\n", temp, hum);
                if (hum > 60 || temp > 30) {
                    setup_pwm(ENA, 1);
                } else {
                    stop_pwm(ENA);
                }
            } else {
                printf("❌ Error leyendo DHT11\n");
            }
            last_dht11_read = get_absolute_time();
        }

        if (sensor1_triggered) {
            sensor1_triggered = false;
            inventario++;
            printf("📦 Producto ingresado. Inventario: %d\n", inventario);
        }
        if (sensor2_triggered) {
            sensor2_triggered = false;
            if (inventario > 0) inventario--;
            printf("📤 Producto retirado. Inventario: %d\n", inventario);
        }

        if (motion_detected) {
            motion_detected = false;
            last_motion_time = get_absolute_time();
            if (!led_on) {
                gpio_put(LED_GPIO, 1);
                led_on = true;
                printf("🏃 Movimiento detectado - LED encendido\n");
            }
        }

        if (led_on && absolute_time_diff_us(last_motion_time, get_absolute_time()) >= 5000000) {
            gpio_put(LED_GPIO, 0);
            led_on = false;
            printf("💡 LED apagado (sin movimiento)\n");
        }

        if (gpio_get(BTN_START) && !motor2_on) {
            setup_pwm(ENA2, 0.07f);
            motor2_on = true;
            printf("🟢 Motor 2 encendido\n");
        } else if (gpio_get(BTN_STOP) && motor2_on) {
            stop_pwm(ENA2);
            motor2_on = false;
        }

        sleep_ms(10);
    }

    return 0;
}
