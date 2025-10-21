#include <Arduino.h>

// --- Pin setup ---
#define BUZZER_PIN 27
#define LED_PIN    18

// --- PWM channels ---
#define BUZZER_CH  0
#define LED_CH     1

// --- PWM setup ---
#define BUZZER_FREQ  2000
#define LED_FREQ     5000
#define BUZZER_RES   10
#define LED_RES      8

void setup() {
  // Setup PWM for buzzer
  ledcSetup(BUZZER_CH, BUZZER_FREQ, BUZZER_RES);
  ledcAttachPin(BUZZER_PIN, BUZZER_CH);

  // Setup PWM for LED
  ledcSetup(LED_CH, LED_FREQ, LED_RES);
  ledcAttachPin(LED_PIN, LED_CH);
}

void loop() {
  // Fade up (LED brighter, buzzer pitch rising)
  for (int d = 0; d <= 255; d++) {
    ledcWrite(LED_CH, d);                            // LED brightness
    int toneFreq = map(d, 0, 255, 500, 2000);        // Map brightness to tone
    ledcWriteTone(BUZZER_CH, toneFreq);              // Change buzzer pitch
    delay(8);                                        // Smooth transition
  }

  // Fade down (LED dimmer, buzzer pitch falling)
  for (int d = 255; d >= 0; d--) {
    ledcWrite(LED_CH, d);
    int toneFreq = map(d, 0, 255, 500, 2000);
    ledcWriteTone(BUZZER_CH, toneFreq);
    delay(8);
  }
}
