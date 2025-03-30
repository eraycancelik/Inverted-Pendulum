#include <ezButton.h>

// Enkoder Pinleri
#define CHA_PIN 8
#define CHB_PIN 12
#define CHZ_PIN 10

// Limit Switch Pinleri
#define RIGHT_SWITCH_PIN 0
#define LEFT_SWITCH_PIN 6

// Enkoder değişkenleri
volatile long pulseCount = 0;
int lastStateCHA;
int currentStateCHA;
int currentStateCHB;

// Limit switch nesneleri
ezButton rightSwitch(RIGHT_SWITCH_PIN);
ezButton leftSwitch(LEFT_SWITCH_PIN);

void setup() {
  Serial.begin(115200);

  pinMode(CHA_PIN, INPUT);
  pinMode(CHB_PIN, INPUT);
  pinMode(CHZ_PIN, INPUT);

  rightSwitch.setDebounceTime(50);
  leftSwitch.setDebounceTime(50);

  attachInterrupt(digitalPinToInterrupt(CHA_PIN), encoderISR, CHANGE);
}

void loop() {
  rightSwitch.loop();
  leftSwitch.loop();

  // Enkoder verisini oku ve yazdır
  Serial.print("Pulse Count: ");
  Serial.println(pulseCount);

  // Index pulse kontrolü
  if (digitalRead(CHZ_PIN) == HIGH) {
    Serial.println("Index Pulse Detected!");
  }

  // Limit switchleri kontrol et
  checkLimitSwitches();

  delay(100);
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
  if (rightSwitch.isReleased()) {
    Serial.println("Right limit switch: TOUCHED - Counter Reset!");
    pulseCount = 0;
  }

  if (leftSwitch.isReleased()) {
    Serial.print("Left limit switch: TOUCHED - Total Steps: ");
    Serial.println(pulseCount);
  }
}