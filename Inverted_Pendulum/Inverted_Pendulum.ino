#include <ezButton.h>
#include <PID_v1.h> // Include the PID library

// ... (Pin tanımlamaları ve diğer değişkenler aynı kalacak) ...
// Enkoder Pinleri
#define CHA_PIN 28
#define CHB_PIN 25
#define CHZ_PIN 30 // Index pini kullanılmıyor ancak tanımı kalabilir

// Limit Switch Pinleri
#define RIGHT_SWITCH_PIN 12
#define LEFT_SWITCH_PIN 24

// Motor Sürücü Pinleri
#define RPWM 0      // PWM çıkışı olan pin (sağa yön)
#define LPWM 10     // PWM çıkışı olan pin (sola yön)
#define R_EN 8      // Sağa yön Enable pin
#define L_EN 6      // Sola yön Enable pin

#define HOMING_PWM 90   // Sola ilk gidiş hızı
#define TRAVEL_PWM 90   // Sağa gidiş hızı (mesafe ölçümü için)
// RETURN_PWM kaldırıldı, PID kontrol edecek

// --- PID Ayarları ---
double Kp = 0.3;
double Ki = 0;
double Kd = 0.02;

double pidSetpoint;
double pidInput;
double pidOutput;

PID myPID(&pidInput, &pidOutput, &pidSetpoint, Kp, Ki, Kd, REVERSE);

#define PID_TOLERANCE 5
// --- PID Ayarları Sonu ---

volatile long pulseCount = 0;
volatile int currentStateCHA;
volatile int currentStateCHB;

ezButton rightSwitch(RIGHT_SWITCH_PIN);
ezButton leftSwitch(LEFT_SWITCH_PIN);

enum State {
  STATE_INITIAL_LEFT,
  STATE_MEASURE_RIGHT,
  STATE_RETURN_TO_CENTER,
  STATE_STOPPED
};

State currentState = STATE_INITIAL_LEFT;

long totalDistancePulses = 0;

void setup() {
  Serial.begin(115200); // Baud rate'i unutmayın!
  Serial.println("Sistem baslatiliyor...");

  pinMode(CHA_PIN, INPUT_PULLUP);
  pinMode(CHB_PIN, INPUT_PULLUP);
  pinMode(CHZ_PIN, INPUT);

  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);

  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  stopMotor();

  rightSwitch.setDebounceTime(50);
  leftSwitch.setDebounceTime(50);

  currentStateCHA = digitalRead(CHA_PIN);
  attachInterrupt(digitalPinToInterrupt(CHA_PIN), encoderISR, CHANGE);

  myPID.SetMode(MANUAL);
  myPID.SetOutputLimits(0, 150);
  myPID.SetSampleTime(10); // PID hesaplama sıklığı 10ms

  Serial.println("Baslangic: Sol limite gidiliyor...");
  delay(1000);
}

void loop() {
  rightSwitch.loop();
  leftSwitch.loop();

  noInterrupts();
  long currentPulse = pulseCount;
  interrupts();
  pidInput = (double)currentPulse;

  switch (currentState) {
    case STATE_INITIAL_LEFT:
      myPID.SetMode(MANUAL);
      moveLeft(HOMING_PWM);
      if (leftSwitch.isReleased()) { // Veya isPressed() hangisi uygunsa
        Serial.println("Sol limit switch'e dokunuldu. Sayac sifirlaniyor.");
        stopMotor();
        delay(200);
        noInterrupts();
        pulseCount = 0;
        interrupts();
        pidInput = 0;
        Serial.println("Sayac sifirlandi. Simdi saga gidiliyor...");
        currentState = STATE_MEASURE_RIGHT;
        // delay(50); // İsteğe bağlı kısa bekleme
      }
      break;

    case STATE_MEASURE_RIGHT:
      myPID.SetMode(MANUAL);
      moveRight(TRAVEL_PWM);
      if (rightSwitch.isReleased()) { // Veya isPressed() hangisi uygunsa
         Serial.print("Sag limit switch'e dokunuldu. Toplam Adim: ");
         noInterrupts();
         long finalPulse = pulseCount;
         interrupts();
         pidInput = (double)finalPulse;

         Serial.println(finalPulse);
         stopMotor();
         delay(200);
         totalDistancePulses = finalPulse;
         pidSetpoint = (double)totalDistancePulses / 2.0;
         Serial.print("Toplam mesafe (puls): ");
         Serial.println(totalDistancePulses);
         Serial.print("Hedef orta nokta (PID Setpoint): ");
         Serial.println(pidSetpoint);
         Serial.println("Merkeze donuluyor (PID ile)...");

         myPID.SetMode(AUTOMATIC);
         currentState = STATE_RETURN_TO_CENTER;
         // delay(50); // İsteğe bağlı kısa bekleme
      }
      break;

    case STATE_RETURN_TO_CENTER:
      myPID.Compute(); // PID hesaplaması yapılıyor
      moveLeft((int)pidOutput); // Hesaplanan değerle motor sürülüyor

      // --- SERIAL PLOTTER İÇİN VERİ GÖNDERME ---
      Serial.print(pidInput);     // Mevcut pozisyonu yazdır
      Serial.print(",");          // Değerleri ayır
      Serial.print(pidSetpoint);  // Hedef pozisyonu yazdır
      Serial.print(",");          // Değerleri ayır
      Serial.println(pidOutput);  // PID çıkışını (PWM) yazdır ve satırı bitir
      // -----------------------------------------

      // Plotter'ın veriyi işlemesi ve loop'un çok hızlı dönmemesi için küçük bir bekleme
      delay(20); // PID örnekleme süresine (10ms) yakın veya biraz fazla olabilir

      // Hedefe varış kontrolü
      if (abs(pidSetpoint - pidInput) <= PID_TOLERANCE) {
         Serial.print("Merkeze ulasildi (hedef: ");
         Serial.print(pidSetpoint);
         Serial.print(", mevcut: ");
         Serial.println(pidInput);
         Serial.println("Hareket tamamlandi. Motor durduruldu.");

         stopMotor();
         myPID.SetMode(MANUAL); // PID'yi kapat
         currentState = STATE_STOPPED;
         // Durduktan sonra plottera veri göndermeyi durdurmak için burada break de edilebilir
         // veya STATE_STOPPED'a geçince zaten göndermeyecektir.
      }
      break; // STATE_RETURN_TO_CENTER sonu

    case STATE_STOPPED:
      stopMotor();
      // Serial Plotter'ı kapatana kadar boş veri göndermemek için burada bekleyebiliriz.
      delay(1000);
      break;
  }
}

// ... (ISR ve motor kontrol fonksiyonları aynı kalacak) ...
// Interrupt Service Routine (ISR) - Enkoder okuma
void encoderISR() {
  int readingCHA = digitalRead(CHA_PIN);
  int readingCHB = digitalRead(CHB_PIN);

  if (readingCHA != currentStateCHA) {
      if (readingCHA == readingCHB) {
          pulseCount++;
      } else {
          pulseCount--;
      }
      currentStateCHA = readingCHA;
  }
}

// Motoru durduran yardımcı fonksiyon
void stopMotor() {
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);
}

// Motoru sağa hareket ettiren yardımcı fonksiyon
void moveRight(int pwmValue) {
  pwmValue = constrain(pwmValue, 0, 255);
  analogWrite(LPWM, 0);
  analogWrite(RPWM, pwmValue);
  digitalWrite(L_EN, HIGH);
  digitalWrite(R_EN, HIGH);
}

// Motoru sola hareket ettiren yardımcı fonksiyon
void moveLeft(int pwmValue) {
  pwmValue = constrain(pwmValue, 0, 255);
  analogWrite(RPWM, 0);
  analogWrite(LPWM, pwmValue);
  digitalWrite(L_EN, HIGH);
  digitalWrite(R_EN, HIGH);
}