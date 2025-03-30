#include <ezButton.h>

#define LIMIT_SWITCH_1 0  // Birinci limit switch pini
#define LIMIT_SWITCH_2 6  // İkinci limit switch pini
#define LED_PIN 13  // Dahili LED pini

ezButton button1(LIMIT_SWITCH_1);
ezButton button2(LIMIT_SWITCH_2);
bool ledState = LOW;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    button1.setDebounceTime(50); // Buton titreşimini önlemek için
    button2.setDebounceTime(50);
}

void loop() {
    button1.loop();
    button2.loop();
    
    if (button1.isReleased() || button2.isReleased()) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
        Serial.println("LED Toggled");
        delay(200); // Yanlış tetiklemeleri önlemek için kısa bekleme
    }
}

