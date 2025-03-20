#include <ezButton.h>

// Enkoder Pinleri
#define CHA_PIN 2
#define CHB_PIN 3
#define CHZ_PIN 4

// Limit Switch Pinleri
#define RIGHT_SWITCH_PIN 6
#define LEFT_SWITCH_PIN 5

// Motor Sürücü Pinleri (BTS7960 PWM ile)
#define RPWM 9   // Sağa PWM (hız kontrolü)
#define LPWM 10  // Sola PWM (hız kontrolü)
#define R_EN 7   // Sağa dönüş Enable
#define L_EN 8   // Sola dönüş Enable

// Enkoder değişkenleri
volatile long pulseCount = 0;
long maxPulseCount = 0;  // Toplam adım sayısını saklamak için

// Limit switch nesneleri
ezButton rightSwitch(RIGHT_SWITCH_PIN);
ezButton leftSwitch(LEFT_SWITCH_PIN);

int lastRightState = LOW;
int lastLeftState = LOW;

void setup() {
  Serial.begin(9600);

  pinMode(CHA_PIN, INPUT);
  pinMode(CHB_PIN, INPUT);
  pinMode(CHZ_PIN, INPUT);
  
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);

  rightSwitch.setDebounceTime(50);
  leftSwitch.setDebounceTime(50);

  attachInterrupt(digitalPinToInterrupt(CHA_PIN), encoderISR, CHANGE);

  // Motor sürücüyü etkinleştir
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  // Başlangıçta motoru yavaş hızda sağa döndür
  moveRight(50);
}

void loop() {
  checkLimitSwitches();
}

// **Enkoder ISR (Sayım işlemi)**
void encoderISR() {
  if (digitalRead(CHA_PIN) == digitalRead(CHB_PIN)) {
    pulseCount++;
  } else {
    pulseCount--;
  }
}

// **Limit switchleri kontrol eden fonksiyon**
void checkLimitSwitches() {
  rightSwitch.loop();
  leftSwitch.loop();

  int rightState = rightSwitch.getState();
  int leftState = leftSwitch.getState();

  // **Sağ limit switch'e basıldığında ters yöne dön**
  if (rightState != lastRightState) {
    if (rightState == HIGH) {
      Serial.println("Right limit switch: TOUCHED - Reverse Motor!");
      pulseCount = 0;
      moveLeft(100);
    }
    lastRightState = rightState;
  }

  // **Sol limit switch'e basıldığında ortalama adımı bul**
  if (leftState != lastLeftState) {
    if (leftState == HIGH) {
      Serial.print("Left limit switch: TOUCHED - Total Steps: ");
      Serial.println(pulseCount);

      maxPulseCount = pulseCount; // Toplam adım sayısını kaydet
      moveToMiddle(); // Ortaya git
    }
    lastLeftState = leftState;
  }
}

// **Motoru sağa döndür (PWM ile)**
void moveRight(int speed) {
  analogWrite(RPWM, speed);
  analogWrite(LPWM, 0);
}

// **Motoru sola döndür (PWM ile)**
void moveLeft(int speed) {
  analogWrite(RPWM, 0);
  analogWrite(LPWM, speed);
}

// **Motoru ortalama adım sayısına kadar götür**
void moveToMiddle() {
  long targetPulse = maxPulseCount / 2;
  Serial.print("Moving to middle step: ");
  Serial.println(targetPulse);

  moveRight(50); // Geri sağa git

  while (pulseCount < targetPulse) {
    delay(5);  // Enkoder okuma süresi
  }

  stopMotor();  // Ortaya gelince motoru durdur
}

// **Motoru durdur**
void stopMotor() {
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);
  Serial.println("Motor Stopped at Middle Position.");
}
