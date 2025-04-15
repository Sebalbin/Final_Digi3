# FoodCare

El interés en este trabajo radica en la automatización de los controles de un entorno que es de necesidad para las personas, con lo cual se facilitaría el proceso y se garantizaría una mayor salubridad en los productos alimenticios.

---

## Descripción del proyecto

**FoodCare** es un sistema embebido diseñado para controlar de forma eficiente y automatizada un entorno de manipulación de alimentos. Este sistema tiene como objetivo:

- Monitorear las condiciones ambientales de una bodega (temperatura y humedad).
- Controlar la entrada y salida de productos mediante conteo automático con sensores de interrupción y una banda transportadora.
- Gestionar la iluminación del entorno según la presencia humana.

### El sistema contempla:

- ✅ Conteo automatizado de productos en entrada y salida.  
- ⚙️ Control de motor DC para banda transportadora.  
- 🌬️ Control de ventilación según humedad.  
- 💡 Iluminación activada por sensor de movimiento.  
- 🌡️ Monitoreo constante de temperatura y humedad.

---

## Requisitos funcionales

### Conteo de productos entrantes y salientes

- El sistema debe detectar productos que ingresan o salen usando sensores infrarrojos.  
- El conteo debe mantenerse actualizado en memoria.

### Control del motor DC

- La banda transportadora será activada mediante un botón.  
- El motor debe funcionar mientras el botón esté activado.

### Lectura ambiental

- Debe medir la temperatura y humedad con un sensor DHT11 en intervalos periódicos.

### Control de ventilador

- El ventilador se activará automáticamente si la humedad supera un umbral definido.

### Iluminación por detección de movimiento

- Cuando se detecte presencia humana, se encenderán los LEDs para iluminar el espacio.

### Visualización o almacenamiento de los datos

- Aunque no se ha especificado una interfaz, se pueden guardar los datos para futuras consultas o análisis (opcional).

---

## Requisitos no funcionales

- 🔢 **Precisión del conteo**: El sistema debe asegurar que no haya errores durante la entrada o salida de productos.  
- 🔄 **Confiabilidad en el procesamiento de datos**: El código debe manejar múltiples sensores sin errores.  
- ⚡ **Respuestas confiables**: Las acciones del sistema deben ser coherentes con los valores leídos.  
- 🔋 **Bajo consumo energético**: Se buscará eficiencia en los componentes, ideal para operación con baterías.  
- 🧠 **Facilidad de uso**: Interfaz intuitiva con botón y sensores, mínima capacitación requerida.  
- 🔧 **Mantenibilidad**: Código modular y documentado, fácil de modificar en el futuro.

---

## Escenario de pruebas

Para validar el sistema se construirá una maqueta que simule un entorno de bodega alimentaria. Las pruebas incluirán:

- ✔️ Simulación de entrada/salida de productos con objetos pasando por los sensores infrarrojos.  
- ✔️ Activación de la banda con un botón físico, verificando el correcto funcionamiento del motor DC.  
- ✔️ Simulación de condiciones ambientales, manipulando el sensor DHT11 con calor y frío.  
- ✔️ Activación del ventilador bajo condiciones de alta humedad.  
- ✔️ Prueba del sensor de movimiento con desplazamientos humanos simulados.  
- ✔️ Monitoreo de todos los datos desde la Raspberry Pi Pico y verificación manual durante las pruebas.

---

## Presupuesto

| Componente                           | Estado      | Costo estimado (COP) |
|-------------------------------------|-------------|-----------------------|
| Sensor de movimiento SR501          | A comprar   | 7,000                 |
| Motor DC                            | Adquirido   | 0                     |
| Sensores infrarrojos x2             | Adquiridos  | 0                     |
| LEDs                                | Adquiridos  | 0                     |
| Sensor DHT11 (temp. y humedad)      | Adquirido   | 0                     |
| Ventilador                          | Adquirido   | 0                     |
| Raspberry Pi Pico                   | Adquirida   | 0                     |
| Material para maqueta               | A comprar   | 10,000                |
| **Total estimado**                  |             | **17,000 COP**        |

**Cobertura de costos:**  
Los componentes han sido cubiertos previamente en otros proyectos o prácticas. Los costos adicionales (sensor de movimiento y maqueta) serán cubiertos por los integrantes del equipo.

---
