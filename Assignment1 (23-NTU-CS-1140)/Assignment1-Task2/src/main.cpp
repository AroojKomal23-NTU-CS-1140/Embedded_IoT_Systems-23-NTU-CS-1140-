/* Name : Arooj Komal
   Reg no. : 23-NTU-CS-1140 
   Section: BSAI 5th
*/



#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Pin setup ---
#define BUTTON_PIN 32
#define LED_PIN 2
#define BUZZER_PIN 18

// --- OLED setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Variables ---
bool ledState = false;             // Keeps track of LED ON/OFF
unsigned long pressStartTime = 0;  // When button was pressed
bool buttonPressed = false;        // Flag for press detection
const unsigned long longPressTime = 1500; // 1.5 seconds

// --- Helper function to update OLED text ---
void showMessage(const char* msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 25);
  display.print(msg);
  display.display();
}

// --- Setup ---
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button reads HIGH when not pressed
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true);
  }

  showMessage("Ready! Press Button");
  digitalWrite(LED_PIN, ledState);
}

// --- Loop ---
void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && !buttonPressed) {
    // Button just pressed
    pressStartTime = millis();
    buttonPressed = true;
  }

  if (buttonState == HIGH && buttonPressed) {
    // Button just released
    unsigned long pressDuration = millis() - pressStartTime;
    buttonPressed = false;

    if (pressDuration < longPressTime) {
      // Short press → toggle LED
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      showMessage(ledState ? "Short Press:\nLED ON" : "Short Press:\nLED OFF");
      Serial.println("Short press detected");
    } else {
      // Long press → play buzzer
      showMessage("Long Press:\nBuzzer Tone!");
      Serial.println("Long press detected");
      tone(BUZZER_PIN, 1000, 500); // 1kHz tone for 500 ms
      delay(500);
      noTone(BUZZER_PIN);
    }
  }
}
