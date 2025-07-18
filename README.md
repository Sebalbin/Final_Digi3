# FoodCare

El interés en este trabajo radica en la automatización de los controles de un entorno que es de necesidad para las personas, con lo cual se facilitaría el proceso y se garantizaría una mayor salubridad en los productos alimenticios.

---

## Descripción del proyecto

**FoodCare** es un sistema embebido diseñado para controlar de forma eficiente y automatizada un entorno de manipulación de alimentos. Este sistema tiene como objetivo:

- Monitorear las condiciones ambientales de una bodega (temperatura y humedad).
- Controlar la entrada y salida de productos mediante conteo automático con sensores de interrupción.
- Gestionar la iluminación del entorno según la presencia humana.

### El sistema contempla:

- ✅ Conteo automatizado de productos en entrada y salida.  
- 🌬️ Control de ventilación según humedad.  
- 💡 Iluminación activada por sensor de movimiento.  
- 🌡️ Monitoreo constante de temperatura y humedad.

---

## Cambios respecto a la propuesta inicial

A continuación, se presenta una tabla con los elementos propuestos inicialmente y los cambios realizados:

| Elemento                         | Propuesta inicial                            | Propuesta final                                |
|----------------------------------|-----------------------------------------------|-------------------------------------------------|
| Conteo de productos              | Sensores IR en entrada y salida              | ✅ Implementado                                 |
| Banda transportadora             | Motor DC activado por botón                  | ❌ No se implementó (por logística de maqueta)  |
| Control de ventilación           | Activación por humedad                       | ✅ Implementado                                 |
| Iluminación por presencia        | Sensor PIR activa iluminación                | ✅ Implementado                                 |
| Sensor ambiental DHT11          | Lectura periódica de temperatura y humedad   | ✅ Implementado                                 |

**Justificación del cambio:**  
La banda transportadora fue descartada debido a limitaciones físicas en la maqueta, priorizando en cambio la fiabilidad de lectura ambiental, conteo, y sensores de movimiento. Todos los demás componentes fueron desarrollados e integrados de acuerdo con lo propuesto.

---

## Diagrama de bloques del hardware

📌 *El sistema está compuesto por los siguientes bloques funcionales*:

- Microcontrolador: Raspberry Pi Pico  
- Sensor de temperatura y humedad: DHT11  
- Sensores infrarrojos (x2): Conteo entrada/salida  
- Sensor PIR: Activación de iluminación  
- Ventilador: Activado por humedad  
- LEDs: Iluminación controlada  
- Botones: Control de activación (pruebas manuales)  

*💡 Diagrama disponible en el directorio `/docs/diagramas` del repositorio.*

---

## Diagrama de flujo del firmware

📌 *Flujo lógico principal del programa:*

1. Inicialización de GPIOs y sensores  
2. Loop principal:
   - Leer sensores IR y actualizar conteo  
   - Leer sensor DHT11  
   - Activar ventilador si humedad > 50%  
   - Detectar movimiento con sensor PIR y activar LEDs  
   - Revisar botón de prueba (si se conecta motor)  

*💡 Diagrama en `/docs/diagrama_flujo.png`*

---

## Análisis del cumplimiento de requisitos

### Requisitos funcionales

| Requisito                            | Cumplido |
|--------------------------------------|----------|
| Conteo de productos IR               | ✅        |
| Lectura de temperatura y humedad     | ✅        |
| Activación de ventilador             | ✅        |
| Iluminación por detección de PIR     | ✅        |
| Activación de motor por botón        | ❌        |

> 🔧 *El motor fue omitido en el prototipo final.*

### Requisitos no funcionales

| Requisito                            | Cumplido |
|--------------------------------------|----------|
| Precisión en conteo                  | ✅        |
| Confiabilidad de procesamiento       | ✅        |
| Bajo consumo energético              | ✅        |
| Respuesta coherente a entradas       | ✅        |
| Facilidad de uso                     | ✅        |
| Mantenibilidad (modularidad del código) | ✅    |

---

## Uso de técnicas de polling e interrupciones

| Funcionalidad                       | Técnica utilizada |
|------------------------------------|-------------------|
| Lectura del sensor DHT11           | Polling           |
| Detección de objetos (IR)          | Polling           |
| Activación por movimiento (PIR)    | Polling           |
| Lectura de botones físicos         | Interrupciones    |

---

## Análisis de costos

### Estimación de producción

| Concepto                            | Costo unitario (COP) |
|-------------------------------------|-----------------------|
| Sensor PIR                          | 7,000                 |
| Sensor IR x2                        | 5,000                 |
| Motor DC                            | 2,000                 |
| LEDs y resistencias                 | 500                   |
| Sensor DHT11                        | 8,000                 |
| Ventilador pequeño                  | 7,000                 |
| Raspberry Pi Pico                   | 33,000                |
| Caja o soporte físico               | 15,000                |
| **Total estimado producción**       | **77,500 COP**        |

---

## Sebastian Balbin Rivera

- Equipo: *FoodCare*
- Fecha: Julio 2025
- Institución: Universidad de Antioquia – Facultad de Ingeniería

---


