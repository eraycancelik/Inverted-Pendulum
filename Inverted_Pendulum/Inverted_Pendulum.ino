#include <ezButton.h>

// Enkoder Pinleri
#define CHA_PIN 2
#define CHB_PIN 3
#define CHZ_PIN 4

// Limit Switch Pinleri
#define RIGHT_SWITCH_PIN 6
#define LEFT_SWITCH_PIN 5

// Enkoder değişkenleri
volatile long pulseCount = 0;
int lastStateCHA;
int currentStateCHA;
int currentStateCHB;

// Limit switch nesneleri
ezButton rightSwitch(RIGHT_SWITCH_PIN);
ezButton leftSwitch(LEFT_SWITCH_PIN);

// Önceki limit switch durumlarını saklayan değişkenler
int lastRightState = LOW;
int lastLeftState = LOW;

void setup() {
  Serial.begin(9600);

  pinMode(CHA_PIN, INPUT);
  pinMode(CHB_PIN, INPUT);
  pinMode(CHZ_PIN, INPUT);

  rightSwitch.setDebounceTime(50);
  leftSwitch.setDebounceTime(50);

  attachInterrupt(digitalPinToInterrupt(CHA_PIN), encoderISR, CHANGE);
}

void loop() {
  // Enkoder verisini oku ve yazdır
  currentStateCHA = digitalRead(CHA_PIN);
  currentStateCHB = digitalRead(CHB_PIN);

  Serial.print("Pulse Count: ");
  Serial.println(pulseCount);

  // Index pulse kontrolü
  if (digitalRead(CHZ_PIN) == HIGH) {
    Serial.println("Index Pulse Detected!");
  }

  // Limit switchleri kontrol et
  checkLimitSwitches();
}

// Interrupt Service Routine (ISR) - Enkoder okuma
void encoderISR() {
  currentStateCHA = digitalRead(CHA_PIN);
  currentStateCHB = digitalRead(CHB_PIN);

  if (currentStateCHA == currentStateCHB) {
    pulseCount++;
  } else {
    pulseCount--;
  }
}

// Limit switchlerin durum değişikliğini kontrol eden fonksiyon
void checkLimitSwitches() {
  rightSwitch.loop();
  leftSwitch.loop();

  int rightState = rightSwitch.getState();
  int leftState = leftSwitch.getState();

  // Sağ limit switch'e basıldığında sayacı sıfırla
  if (rightState != lastRightState) {
    if (rightState == HIGH) {
      Serial.println("Right limit switch: TOUCHED - Counter Reset!");
      pulseCount = 0;
    }
    lastRightState = rightState;
  }

  // Sol limit switch'e basıldığında toplam adımı yazdır
  if (leftState != lastLeftState) {
    if (leftState == HIGH) {
      Serial.print("Left limit switch: TOUCHED - Total Steps: ");
      Serial.println(pulseCount);
    }
    lastLeftState = leftState;
  }
}
