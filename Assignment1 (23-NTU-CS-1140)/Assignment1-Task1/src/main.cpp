/* Name: Arooj Komal
   Reg no. : 23-NTU-CS-1140
   Section: BSAI 5th
*/


#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---- Pin Setup ----
#define BUTTON_MODE 32    // Button to change LED mode
#define BUTTON_RESET 33   // Button to reset to OFF
#define LED1 2
#define LED2 4
#define LED3 18

// ---- OLED Setup ----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---- Global Variables ----
int mode = 0;
int lastButtonState = HIGH;

// ---- Function to Update OLED Display ----
void updateDisplay(const char* message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.print("Current Mode:");
  display.setCursor(0, 40);
  display.print(message);
  display.display();
}

// ---- Setup ----
void setup() {
  pinMode(BUTTON_MODE, INPUT_PULLUP);
  pinMode(BUTTON_RESET, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  Serial.begin(9600);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("ESP32 LED Controller");
  display.setCursor(0, 30);
  display.println("Press button to start");
  display.display();

  Serial.println("ESP32 LED Mode Controller Ready!");
}

// ---- Main Loop ----
void loop() {
  int buttonState = digitalRead(BUTTON_MODE);
  int resetState = digitalRead(BUTTON_RESET);

  // --- Change Mode Button ---
  if (buttonState == LOW && lastButtonState == HIGH) {
    mode++;
    if (mode > 3) mode = 0;
    Serial.print("Mode changed to: ");
    Serial.println(mode);
    delay(200);
  }

  // --- Reset Button ---
  if (resetState == LOW) {
    mode = 0;
    Serial.println("Reset to OFF");
    delay(200);
  }

  lastButtonState = buttonState;

  // --- LED Behavior Based on Mode ---
  switch (mode) {
    case 0: // OFF
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      updateDisplay("OFF");
      break;

    case 1: // Alternate Blink
      updateDisplay("Alternate Blink");
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      delay(300);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      delay(300);
      break;

    case 2: // All ON
      updateDisplay("All ON");
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      break;

    case 3: // Fade LED1
      updateDisplay("PWM Fade");
      for (int d = 0; d <= 255; d++) {
        analogWrite(LED1, d);
        delay(5);
        analogWrite(LED2, d);
        delay(5);
      }
      for (int d = 255; d >= 0; d--) {
        analogWrite(LED1, d);
        delay(5);
        analogWrite(LED2, d);
        delay(5);
      }
      break;
  }
}
