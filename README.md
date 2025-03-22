# PRÁCTICA SENSOR DE HUMEDAD

**Universidad de Colima**  
**Facultad de Telemática**  
**Ingeniería de Software**  

**Equipo conformado por**:  
- Sergio Sebastian Velasco Torres  
- Lissandra Duran Romero  
- Ian Anthony Perez Gonzalez  

Este proyecto utiliza un sensor de humedad, LEDs RGB, un buzzer, un relé y una pantalla LCD para monitorear y controlar automáticamente el riego de plantas basado en la humedad del suelo.

## Conexiones de los Pines

- **Sensor de Humedad**: Conectado al pin analógico `A0`.
- **LED RGB**:
  - Rojo: Pin digital `3`.
  - Verde: Pin digital `6`.
  - Azul: Pin digital `5`.
- **Botón**: Conectado al pin digital `7` con resistencia pull-up interna.
- **Relé**: Conectado al pin digital `8` para controlar la bomba de agua.
- **Buzzer**: Conectado al pin digital `9`.
- **Pantalla LCD**: Conectada a través de I2C (dirección `0x27`).

## Descripción de las Funciones

### `encenderRiegoAutomatizado()`
Activa el modo de riego automático, cambia el color del LED RGB a púrpura, reproduce una melodía y muestra un mensaje en el monitor serial.

### `apagarRiegoAutomatizado()`
Desactiva el modo de riego automático, apaga el LED RGB, reproduce una melodía y muestra un mensaje en el monitor serial.

### `getSoilmoisture()`
Lee el valor analógico del sensor de humedad y lo convierte a un porcentaje de humedad del suelo.

### `ledIndicator(uint8_t data)`
Cambia el color del LED RGB según el nivel de humedad:
- Rojo: Menor al 25%.
- Amarillo: Entre 25% y 50%.
- Blanco: Entre 50% y 75%.
- Azul: Entre 75% y 100%.
- Verde: Mayor al 100%.

### `checkHumidity(uint8_t data)`
Devuelve `true` si la humedad es mayor al 80%, indicando que el suelo está suficientemente húmedo.

### `playSong(uint8_t song)`
Reproduce diferentes melodías según el parámetro `song`:
- `1`: Melodía para activar el riego automático.
- `2`: Melodía para desactivar el riego automático.
- `3`: Melodía para indicar que la bomba está activada.

### `drainWater(bool humidityFlag)`
Activa la bomba de agua si la humedad es menor al 80% (`humidityFlag == false`). También reproduce una melodía cuando la bomba se activa.

### `showValue(uint8_t data)`
Muestra los valores de humedad analógica y digital en el monitor serial y en la pantalla LCD.

### `setup()`
Inicializa los pines, la pantalla LCD, y las variables necesarias para el funcionamiento del sistema.

## Descripción del Loop

### `loop()`
- **Si el botón está presionado**:
  - Activa o desactiva el modo de riego automático.
  - Si el modo automático está activado, revisa la humedad y controla la bomba.
  - Si el modo automático está desactivado, actualiza los LEDs y muestra los valores de humedad.
- **Si el botón no está presionado**:
  - En modo manual, actualiza los LEDs y muestra los valores de humedad si hay cambios.
  - En modo automático, revisa la humedad y controla la bomba.

El `loop` asegura que el sistema responda continuamente a los cambios en la humedad del suelo y a la interacción del usuario mediante el botón.