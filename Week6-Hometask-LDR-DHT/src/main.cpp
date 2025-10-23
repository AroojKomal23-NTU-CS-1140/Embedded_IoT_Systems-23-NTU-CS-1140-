#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// --- Pin configuration ---
#define DHTPIN 14          // DHT data pin
#define DHTTYPE DHT11      // Change to DHT22 if you're using DHT22
#define LDR_PIN 34         // Analog pin connected to LDR

#define SDA_PIN 21         // I2C SDA
#define SCL_PIN 22         // I2C SCL

// --- OLED setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- DHT sensor setup ---
DHT dht(DHTPIN, DHTTYPE);

// --- Setup function ---
void setup() {
  Serial.begin(115200);
  Serial.println("Initializing sensors...");

  // Initialize I2C and OLED
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed!");
    for (;;);
  }

  // Initialize DHT sensor
  dht.begin();

  // Display startup message
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();
  delay(1500);
}

// --- Main loop ---
void loop() {
  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read LDR value
  int ldrValue = analogRead(LDR_PIN);
  float voltage = (ldrValue / 4095.0) * 3.3;  // Convert ADC to voltage

  // Check DHT read errors
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // --- Print to Serial Monitor ---
  Serial.println("--------------------------------------------------");
  Serial.printf("Temperature: %.1f °C | Humidity: %.1f %% | LDR ADC: %d | Voltage: %.2f V\n",
                temperature, humidity, ldrValue, voltage);

  // --- Display on OLED ---
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("=== Sensor Readings ===");

  display.setCursor(0, 16);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.setCursor(0, 28);
  display.print("Humidity: ");
  display.print(humidity, 1);
  display.println(" %");

  display.setCursor(0, 40);
  display.print("LDR ADC: ");
  display.println(ldrValue);

  display.setCursor(0, 52);
  display.print("Volt: ");
  display.print(voltage, 2);
  display.println(" V");

  display.display();

  delay(2000);  // Update every 2 seconds
}
