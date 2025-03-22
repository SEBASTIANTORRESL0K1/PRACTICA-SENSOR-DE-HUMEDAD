#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int sensorpin = A0;
const int pinRed = 3;
const int pinGreen = 6;
const int pinBlue = 5;
const int btn = 7;
const int pinRelay = 8;
int sensor;
int mapeo;
bool relay;
bool riegoAutom;
uint8_t valorPasado;
uint8_t valorActual;
bool btnPress = false;

const int delayTime = 1000;
const int buzzerPin = 9;
byte gota[] = {
  B00100,
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000
};
byte rayo[] = {
  B11110,
  B01110,
  B00111,
  B01110,
  B11100,
  B01111,
  B00110,
  B00100
};

void encenderRiegoAutomatizado() {
  playSong(1);
  analogWrite(pinRed, 76);
  analogWrite(pinGreen, 40);
  analogWrite(pinBlue, 130);
  lcd.clear();
  Serial.println("Modo Automatico encendido");
  Serial.print("Valor ");
  Serial.println(riegoAutom);
  riegoAutom = true;
  Serial.println(riegoAutom);

  delay(500);
  //regar hasta que sea mayor al 85%
  //buzzer
};
void apagarRiegoAutomatizado() {
  analogWrite(pinRed, 0);
  analogWrite(pinGreen, 0);
  analogWrite(pinBlue, 0);
  Serial.println("Modo Automatico apagado");
  Serial.println(riegoAutom);
  riegoAutom = false;
  Serial.println(riegoAutom);
  playSong(2);

  //activará todas las funciones

  delay(500);
};

uint8_t getSoilmoisture() {
  // Obtiene la lectura del sensor y retorna el porcentaje.
  sensor = analogRead(sensorpin);
  uint8_t moisturePercentage = map(sensor, 1016,332 , 0, 100);
  return moisturePercentage;
};

void ledIndicator(uint8_t data) {
  //Obtiene cómo parámetro el resultado de getSoilmoisture y realiza la
  //lógica necesaria para cambiar de color los LEDs.
  if (data < 25) {
    //rojo
    analogWrite(pinRed, 255);
    analogWrite(pinGreen, 0);
    analogWrite(pinBlue, 0);
  }
  if (data > 24 && data < 50) {
    //amarillo
    analogWrite(pinRed, 255);
    analogWrite(pinGreen, 255);
    analogWrite(pinBlue, 0);
  }
  if (data > 49 && data < 75) {
    //blanco
    analogWrite(pinRed, 255);
    analogWrite(pinGreen, 255);
    analogWrite(pinBlue, 255);
  }
  if (data > 74 && data < 100) {
    //azul
    analogWrite(pinRed, 0);
    analogWrite(pinGreen, 0);
    analogWrite(pinBlue, 255);
  }
  if (data > 99) {
    //verde
    analogWrite(pinRed, 0);
    analogWrite(pinGreen, 255);
    analogWrite(pinBlue, 0);
  }
}

bool checkHumidity(uint8_t data) {
  //Devuelve un verdadero cuando la humedad es la adecuada, es
  //decir mayor a 80%.
  bool humedad;
  if (data > 80) {
    humedad = true;
  } else {
    humedad = false;
  }
  return humedad;
}
void playSong(uint8_t song) {
  switch (song) {
    case 1:
      // Reproducir la primera melodía
      tone(buzzerPin, 262, 500);  // Do (C4) durante 500 ms
      delay(500);
      tone(buzzerPin, 294, 500);  // Re (D4) durante 500 ms
      delay(500);
      tone(buzzerPin, 330, 500);  // Mi (E4) durante 500 ms
      delay(500);
      break;

    case 2:
      // Reproducir la segunda melodía
      tone(buzzerPin, 392, 500);  // Sol (G4) durante 500 ms
      delay(500);
      tone(buzzerPin, 440, 500);  // La (A4) durante 500 ms
      delay(500);
      tone(buzzerPin, 494, 500);  // Si (B4) durante 500 ms
      delay(500);
      break;

    case 3:
      // Reproducir la tercera melodía
      tone(buzzerPin, 523, 500);  // Do (C5) durante 500 ms
      delay(500);
      tone(buzzerPin, 587, 500);  // Re (D5) durante 500 ms
      delay(500);
      tone(buzzerPin, 659, 500);  // Mi (E5) durante 500 ms
      delay(500);
      break;

    default:
      // Si el número de canción no es válido, no hacer nada
      break;
  }
}
void drainWater(bool humidityFlag) {

  //Activa la bomba cuando la humedad está por debajo del 80%.
  if (humidityFlag == false) {
    digitalWrite(pinRelay, HIGH);
    Serial.println(valorActual);
    playSong(3);

  } else {
    digitalWrite(pinRelay, LOW);
  }
}
void showValue(uint8_t data) {
  Serial.print("Valor Analogico: ");
  Serial.println(map(data, 0, 100, 1016, 332));
  Serial.print("Valor Digital: ");
  Serial.print(data);
  Serial.println("%");
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Analogico: ");
  lcd.print(map(data, 0, 100, 1016, 332));
  lcd.setCursor(0, 0);
  lcd.print(data);
  lcd.print("%");
  lcd.setCursor(15, 0);
  lcd.write(0);
  lcd.setCursor(15, 1);
  lcd.write(1);
}
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, gota);
  lcd.createChar(1, rayo);
  Serial.begin(9600);
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pinRelay, OUTPUT);
  relay = false;
  riegoAutom = false;
  valorPasado = 0;
}


void loop() {
  if (digitalRead(btn) == LOW) {
    if (riegoAutom == false) {
      encenderRiegoAutomatizado();
      //revisar humedad
      drainWater(checkHumidity(getSoilmoisture()));
    } else {
      apagarRiegoAutomatizado();
      valorActual = getSoilmoisture();


      ledIndicator(valorActual);
      showValue(valorActual);
      valorPasado = valorActual;
    }
  } else {



    if (riegoAutom == false) {
      valorActual = getSoilmoisture();
      if (valorActual != valorPasado) {
        ledIndicator(valorActual);
        showValue(valorActual);
        valorPasado = valorActual;
      }
    } else {
      drainWater(checkHumidity(getSoilmoisture()));

      //revisar humedad
    }
  }
}
