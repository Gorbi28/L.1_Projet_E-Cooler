Voici un exemple de code où on ne transmet à l’OLED que des entiers, en tronquant la partie décimale. Vous aurez ainsi :

Temp : affichée sous forme d’entier (ex. 23 °C)

Hum  : affichée sous forme d’entier (ex. 45 %)

Servo activé si T > 20 °C, désactivé sinon


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
  display.printFixed(0,  8, buf, STYLE_NORMAL);

  snprintf(buf, sizeof(buf), "Hum:  %2d %%", hum_i);
  display.printFixed(0, 16, buf, STYLE_NORMAL);

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

  delay(1000);
}

Explications des changements

1. Cast en int

int temp_i = (int)temp_f;
int hum_i  = (int)hum_f;

On ne garde que la partie entière.


2. snprintf avec %2d
Permet d’afficher un espace si nombre à un seul chiffre, pour un alignement propre.


3. Affichage sur OLED
Toujours via printFixed(), mais en lui passant une chaîne contenant uniquement des chiffres entiers.



Avec ça, vous aurez bien “Temp: 23 C” et “Hum: 45 %” sur l’écran, sans le “?” dû à l’impossibilité d’afficher les décimales.

