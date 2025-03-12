// Pin tanımlamaları
#define CHA_PIN 2
#define CHB_PIN 3
#define CHZ_PIN 4

// Değişkenler
volatile long pulseCount = 0;
int lastStateCHA;
int currentStateCHA;
int currentStateCHB;

void setup() {
  // Seri iletişimi başlat
  Serial.begin(9600);

  // Pin modlarını ayarla
  pinMode(CHA_PIN, INPUT);
  pinMode(CHB_PIN, INPUT);
  pinMode(CHZ_PIN, INPUT);

  // Interrupt'ı başlat: CHA_PIN'deki değişiklikleri tetikle
  attachInterrupt(digitalPinToInterrupt(CHA_PIN), encoderISR, CHANGE);
}

void loop() {
  // Ch A ve Ch B durumlarını oku
  currentStateCHA = digitalRead(CHA_PIN);
  currentStateCHB = digitalRead(CHB_PIN);

  // Pulse Count'ı yazdır
  Serial.print("Pulse Count: ");
  Serial.println(pulseCount);
  
  // Ch Z (Index pulse) kontrolü
  if (digitalRead(CHZ_PIN) == HIGH) {
    Serial.println("Index Pulse Detected!");
  }

  // Diğer işlemleri burada yapabilirsiniz
}

// Interrupt Service Routine (ISR)
void encoderISR() {
  currentStateCHA = digitalRead(CHA_PIN);
  currentStateCHB = digitalRead(CHB_PIN);

  // Ch A ve Ch B durumlarına göre yönü belirle
  if (currentStateCHA == currentStateCHB) {
    pulseCount--;
  } else {
    pulseCount++;
  }
}
