#include <Wire.h>
#include "SHTSensor.h"                // Capteur SHT
#include "lcdgfx.h"                   // Écran OLED SSD1306
#include <Servo.h>                    // Servo

// --- CONFIGURATION OLED I2C ---
DisplaySSD1306_128x64_I2C display(-1);

// --- CONFIGURATION CAPTEUR SHT ---
SHTSensor sht;

// --- CONFIGURATION SERVO ---
Servo myservo;
const int SERVO_PIN = A2;            // Broche du servo

void setup() {
  // Initialisation série (pour debug éventuel)
  Serial.begin(115200);

  // I2C et capteur SHT
  Wire.begin();
  sht.init();
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM);

  // OLED
  display.begin();
  display.fill(0x00);
  display.setFixedFont(ssd1306xled_font6x8);

  // Servo
  myservo.attach(SERVO_PIN);
  myservo.write(0);  // Position “désactivée” au démarrage
}

void loop() {
  // 1) Lecture du capteur
  sht.readSample();
  float temperature = sht.getTemperature();
  float humidity    = sht.getHumidity();

  // 2) Mise à jour de l’écran
  display.fill(0x00);  // Effacer l’écran
  char buf[20];

  // Température
  snprintf(buf, sizeof(buf), "Temp: %.1f C", temperature);
  display.printFixed(0,  8, buf, STYLE_NORMAL);

  // Humidité
  snprintf(buf, sizeof(buf), "Hum:  %.1f %%", humidity);
  display.printFixed(0, 16, buf, STYLE_NORMAL);

  // 3) Contrôle du servo
  if (temperature > 20.0) {
    myservo.write(180);   // Active
  } else {
    myservo.write(0);     // Désactive
  }

  // (facultatif) debug série
  Serial.print("T="); Serial.print(temperature);
  Serial.print("C  H="); Serial.print(humidity);
  Serial.println("%");

  delay(1000);  // 1 seconde entre chaque boucle
}
