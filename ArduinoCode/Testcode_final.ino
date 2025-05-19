#include <Wire.h>
#include "SHTSensor.h"                // Capteur SHT
#include "lcdgfx.h"                   // Écran OLED SSD1306
#include <Servo.h>                    // Servo

// --- CONFIG OLED I2C ---
DisplaySSD1306_128x64_I2C display(-1);

// --- CONFIG CAPTEUR SHT ---
SHTSensor sht;

// --- CONFIG SERVO ---
Servo myservo;
const int SERVO_PIN = A2;            // Broche servo

void setup() {
  // Série pour debug
  Serial.begin(115200);

  // I2C + SHT
  Wire.begin();
  sht.init();
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM);

  // OLED
  display.begin();
  display.fill(0x00);
  display.setFixedFont(ssd1306xled_font6x8);

  // Servo
  myservo.attach(SERVO_PIN);
  myservo.write(0);
}

void loop() {
  // Lecture
  sht.readSample();
  float temp_f = sht.getTemperature();
  float hum_f  = sht.getHumidity();

  // Conversion en entier
  int temp_i = (int)temp_f;
  int hum_i  = (int)hum_f;

  // Affichage OLED
  display.fill(0x00);

  char buf[16];
  snprintf(buf, sizeof(buf), "Temp: %2d C", temp_i);
  display.printFixedN(0,  8, buf, STYLE_NORMAL,FONT_SIZE_2X);

  snprintf(buf, sizeof(buf), "Hum:  %2d %%", hum_i);
  display.printFixedN(0, 32, buf, STYLE_NORMAL, FONT_SIZE_2X);

  // Contrôle servo
  if (temp_i > 20) {
    myservo.write(180);
  } else {
    myservo.write(0);
  }

  // Debug série
  Serial.print("T="); Serial.print(temp_i);
  Serial.print("C  H="); Serial.print(hum_i);
  Serial.println("%");

  delay(5000);
}
