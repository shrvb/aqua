#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>

Adafruit_BMP280 bmp; // Create an instance of the BMP280 sensor
// Initialize the LiquidCrystal_I2C object with the I2C address and dimensions
LiquidCrystal_I2C lcd(0x27, 16, 2); // Address 0x27, 16 columns, 2 rows

void setup() {
  // Start the Serial Monitor
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // Wait for Serial Monitor to open
  }

  Serial.println(F("BMP280 Sensor Test"));

  // Initialize the BMP280 sensor
  if (!bmp.begin(0x76)) { // 0x76 is the I2C address of the BMP280
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  // Configure the BMP280 settings
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,   // Operating Mode.
                  Adafruit_BMP280::SAMPLING_X2,  // Temp. oversampling
                  Adafruit_BMP280::SAMPLING_X16, // Pressure oversampling
                  Adafruit_BMP280::FILTER_X16,   // Filtering.
                  Adafruit_BMP280::STANDBY_MS_500); // Standby time.

  // Initialize the LCD
  lcd.begin();
  lcd.backlight();
  lcd.print("BMP280 & LCD"); // Display a startup message
  delay(2000); // Wait for 2 seconds
  lcd.clear();
}

void loop() {
  // Read temperature and pressure data
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();

  // Display temperature and pressure on the LCD
  lcd.setCursor(0, 0); // Set cursor to the first row
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1); // Set cursor to the second row
  lcd.print("Press: ");
  lcd.print(pressure / 100.0); // Convert Pa to hPa
  lcd.print(" hPa");

  // Print the data to the Serial Monitor
  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.println(F(" °C"));

  Serial.print(F("Pressure: "));
  Serial.print(pressure / 100.0); // Convert Pa to hPa
  Serial.println(F(" hPa"));

  // Wait for 2 seconds before the next reading
  delay(2000);
}
