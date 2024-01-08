#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LedControl.h>

// Definiciones de pines
#define TRIG_PIN 6
#define ECHO_PIN 5
#define SMALL_BTN_PIN 2
#define MEDIUM_BTN_PIN 3
#define LARGE_BTN_PIN 4
#define CANCEL_BTN_PIN 7
#define BUZZER_PIN 11
#define BLUE_LED_PIN 8
#define RED_LED_PIN 9 // Corregido el número del pin
#define GREEN_LED_PIN 10

// Variables globales
LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección I2C de la LCD
LedControl lc = LedControl(12, 11, 10, 1); // Pines para MAX7219

int distance = 0;
bool filling = false;

void setup() {
  pinMode(SMALL_BTN_PIN, INPUT_PULLUP);
  pinMode(MEDIUM_BTN_PIN, INPUT_PULLUP);
  pinMode(LARGE_BTN_PIN, INPUT_PULLUP);
  pinMode(CANCEL_BTN_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Dispensador");

  lc.shutdown(0, false); // Despierta el MAX7219
  lc.setIntensity(0, 8); // Establece el brillo
  lc.clearDisplay(0); // Limpia la pantalla del MAX7219
}

void loop() {
  // Verificar estado de los pulsadores
  if (digitalRead(SMALL_BTN_PIN) == LOW) {
    // Acciones para dispensado tamaño pequeño
    // Por ejemplo, activar la matriz LED con una animación corta
    displayAnimation();
  } else if (digitalRead(MEDIUM_BTN_PIN) == LOW) {
    // Acciones para dispensado tamaño mediano
    // Por ejemplo, activar la matriz LED con una animación mediana
    displayAnimation();
  } else if (digitalRead(LARGE_BTN_PIN) == LOW) {
    // Acciones para dispensado tamaño grande
    // Por ejemplo, activar la matriz LED con una animación larga
    displayAnimation();
  } else if (digitalRead(CANCEL_BTN_PIN) == LOW) {
    // Acciones para cancelar el dispensado
    // Por ejemplo, detener la animación de la matriz LED
    clearMatrix();
  }

  // Medir distancia con el sensor HC-SR04
  distance = measureDistance();

  // Mostrar información en la LCD
  lcd.setCursor(0, 1);
  lcd.print("Distancia: ");
  lcd.print(distance);
  lcd.print(" cm");

  // Controlar LEDS según la distancia detectada
  if (distance < 10) {
    digitalWrite(BLUE_LED_PIN, HIGH);
    filling = true;
  } else {
    digitalWrite(BLUE_LED_PIN, LOW);
    if (filling) {
      digitalWrite(RED_LED_PIN, HIGH);
      digitalWrite(GREEN_LED_PIN, HIGH);
      tone(BUZZER_PIN, 1000); // Activar el buzzer
      delay(500); // Sonido por 0.5 segundos
      noTone(BUZZER_PIN); // Apagar el buzzer
      filling = false;
    }
  }

  delay(500); // Ajustar el tiempo según sea necesario
}

int measureDistance() {
  // Código para medir la distancia con el sensor HC-SR04
  // ...
  return random(5, 30); // Valor de distancia aleatorio para fines de ejemplo
}

void displayAnimation() {
  // Mostrar una animación en la matriz LED (modificar según la animación deseada)
  byte frames[][8] = {
    {B10000000, B01000000, B00100000, B00010000, B00001000, B00000100, B00000010, B00000001},
    {B00100000, B01111000, B11111111, B11111110, B01111111, B00011100, B00011100, B00111000},
    // ...
    // Aquí se colocarían los frames de la animación
    // ...
    {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000},
  };
  int frameCount = sizeof(frames) / sizeof(frames[0]);

  for (int i = 0; i < frameCount; i++) {
    for (int row = 0; row < 8; row++) {
      lc.setRow(0, row, frames[i][row]);
    }
    delay(800); // Ajustar velocidad de animación
  }
}

void clearMatrix() {
  // Limpiar la matriz LED
  lc.clearDisplay(0);
}
