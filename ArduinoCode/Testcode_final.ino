#include <Wire.h>
#include "SHTSensor.h"                // Capteur SHT
#include "lcdgfx.h"                   // Affichage OLED SSD1306
#include <Servo.h>                    // Servo motor

// --- CONFIGURATION OLED ---
DisplaySSD1306_128x64_I2C display(-1);

// --- CONFIGURATION CAPTEUR SHT ---
SHTSensor sht;

// --- CONFIGURATION SERVO ---
Servo myservo;
const int SERVO_PIN = A2;            // Pin à laquelle est connecté le servo

void setup() {
  // Initialisation série (optionnelle pour debug)
  Serial.begin(115200);

  // Initialisation I2C et capteur
  Wire.begin();
  sht.init();
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM);

  // Initialisation de l’afficheur OLED
  display.begin();
  display.fill(0x00); 
  display.setFixedFont(ssd1306xled_font6x8);

  // Initialisation du servo
  myservo.attach(SERVO_PIN);
  myservo.write(0);  // Position initiale
}

void loop() {
  // Lecture du capteur
  sht.readSample();
  float temperature = sht.getTemperature(); 
  float humidity    = sht.getHumidity();

  // Affichage sur OLED
  display.fill(0x00);  // Efface l’écran
  char buf[20];
  // Affiche la température
  snprintf(buf, sizeof(buf), "Temp: %.1f C", temperature);
  display.printFixed(0,  8, buf, STYLE_NORMAL);
  // Affiche l’humidité
  snprintf(buf, sizeof(buf), "Hum:  %.1f %%", humidity);
  display.printFixed(0, 16, buf, STYLE_NORMAL);

  // Mise à jour visible immédiate
  // (lcdgfx met à jour à chaque print, pas besoin de display())

  // Contrôle du servo : si T > 20 °C, position 180° ; sinon 0°
  if (temperature > 20.0) {
    myservo.write(180);
  } else {
    myservo.write(0);
  }

  // Pour debug sur port série
  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print(" C  Humidity: "); Serial.print(humidity); Serial.println(" %");

  delay(1000);  // 1 seconde entre chaque lecture/affichage
}
