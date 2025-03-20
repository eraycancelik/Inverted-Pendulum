#include <ezButton.h>

// Enkoder Pinleri
#define CHA_PIN 2
#define CHB_PIN 3
#define CHZ_PIN 4

// Limit Switch Pinleri
#define RIGHT_SWITCH_PIN 6
#define LEFT_SWITCH_PIN 5

// Motor Sürücü Pinleri
#define RPWM 10      // PWM çıkışı olan pin (sağa yön)
#define LPWM 11      // PWM çıkışı olan pin (sola yön)
#define R_EN 8       // Sağa yön Enable pin
#define L_EN 9       // Sola yön Enable pin

#define runPWM 95    // Motor hızı

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

bool direction = true; // true -> Sağa, false -> Sola

void setup() {
  Serial.begin(115200);

  pinMode(CHA_PIN, INPUT);
  pinMode(CHB_PIN, INPUT);
  pinMode(CHZ_PIN, INPUT);

  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);

  rightSwitch.setDebounceTime(50);
  leftSwitch.setDebounceTime(50);

  attachInterrupt(digitalPinToInterrupt(CHA_PIN), encoderISR, CHANGE);
  delay(3000); 
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

  // Motor yönünü belirle
  if (direction) {
    analogWrite(RPWM, runPWM);
    analogWrite(LPWM, 0);
  } else {
    analogWrite(RPWM, 0);
    analogWrite(LPWM, runPWM);
  }

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
  int rightState = rightSwitch.getState();
  int leftState = leftSwitch.getState();

  // Sağ limit switch'e basıldığında sayacı sıfırla ve sola dön
  if (rightSwitch.isReleased()) {
    Serial.println("Right limit switch: TOUCHED - Counter Reset!");
    pulseCount = 0;
    direction = false; // Sola dön
  }

  // Sol limit switch'e basıldığında toplam adımı yazdır ve sağa dön
  if (leftSwitch.isReleased()) {
    Serial.print("Left limit switch: TOUCHED - Total Steps: ");
    Serial.println(pulseCount);
    direction = true; // Sağa dön
  }
}
