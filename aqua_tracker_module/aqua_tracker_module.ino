#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define LED_PIN 2

Adafruit_BMP280 bmp;

float ambientPressure = 0;
float bottleArea = 50.0;

void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(LED_PIN, OUTPUT);

  Wire.begin(8, 9); // SDA=GPIO8, SCL=GPIO9

  Serial.println("BMP280 initialising...");

  if (!bmp.begin(0x76)) {
    Serial.println("Not found at 0x76, trying 0x77...");
    if (!bmp.begin(0x77)) {
      Serial.println("BMP280 not found. Check wiring.");
      // Blink rapidly to signal error
      while (1) {
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
      }
    }
  }

  Serial.println("BMP280 found.");

  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500
  );

  delay(1000);

  ambientPressure = bmp.readPressure();
  Serial.print("Baseline: ");
  Serial.print(ambientPressure);
  Serial.println(" Pa");
  Serial.println("--- Place bottle over sensor and add water ---");

  // Three slow blinks = ready
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }
}

void loop() {
  float rawP   = bmp.readPressure();
  float netP   = rawP - ambientPressure;
  float height = netP / (1000.0 * 9.81);
  float volume = height * bottleArea * 10000.0;

  Serial.print("Temp: ");
  Serial.print(bmp.readTemperature(), 1);
  Serial.print(" C  |  Raw: ");
  Serial.print(rawP, 1);
  Serial.print(" Pa  |  Net: ");
  Serial.print(netP, 2);
  Serial.print(" Pa  |  Vol: ");
  Serial.print(volume, 1);
  Serial.println(" ml");

  // Single blink per reading
  digitalWrite(LED_PIN, HIGH);
  delay(50);
  digitalWrite(LED_PIN, LOW);

  delay(2000);
}
