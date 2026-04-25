#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>

Adafruit_BMP280 bmp;
LiquidCrystal_I2C lcd(0x27, 16, 2);

float ambientPressure = 0;
float bottleArea = 50.0; // cm² — adjust for your bottle

void setup() {
  Serial.begin(115200);
  delay(2000);

  Wire.begin(8, 9); // SDA=GPIO8, SCL=GPIO9

  // Init LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Aqua Tracker");
  lcd.setCursor(0, 1);
  lcd.print("Initialising...");
  delay(1500);
  lcd.clear();

  // Init BMP280
  Serial.println("BMP280 initialising...");

  if (!bmp.begin(0x76)) {
    Serial.println("Not found at 0x76, trying 0x77...");
    if (!bmp.begin(0x77)) {
      Serial.println("BMP280 not found. Check wiring.");
      lcd.setCursor(0, 0);
      lcd.print("BMP280 Error");
      lcd.setCursor(0, 1);
      lcd.print("Check wiring");
      while (1) delay(100);
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

  // Record baseline
  ambientPressure = bmp.readPressure();

  Serial.print("Baseline: ");
  Serial.print(ambientPressure);
  Serial.println(" Pa");

  lcd.setCursor(0, 0);
  lcd.print("Baseline set");
  lcd.setCursor(0, 1);
  lcd.print(ambientPressure, 1);
  lcd.print(" Pa");
  delay(2000);
  lcd.clear();
}

void loop() {
  float rawP   = bmp.readPressure();
  float netP   = rawP - ambientPressure;
  float height = netP / (1000.0 * 9.81);
  float volume = height * bottleArea * 10000.0;

  // Serial output
  Serial.print("Temp: ");
  Serial.print(bmp.readTemperature(), 1);
  Serial.print(" C  |  Net: ");
  Serial.print(netP, 2);
  Serial.print(" Pa  |  Vol: ");
  Serial.print(volume, 1);
  Serial.println(" ml");

  // LCD row 1 — volume
  lcd.setCursor(0, 0);
  lcd.print("Vol: ");
  lcd.print(volume, 1);
  lcd.print(" ml   "); // trailing spaces clear old chars

  // LCD row 2 — temperature
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(bmp.readTemperature(), 1);
  lcd.print(" C   ");

  delay(2000);
}
