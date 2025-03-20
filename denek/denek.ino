#define PWM_PIN 7  // PWM çıkışı olan pin
#define DIR_PIN 8  // Yön pini

void setup() {
  // Pinleri çıkış olarak ayarla
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  // Hızı çok düşük yapalım (çok yavaş hareket etsin)
  analogWrite(PWM_PIN, 100);  // Hız 0 ile 255 arasında, burada 10 çok yavaş hareket eder
}

void loop() {
  // 3 saniye bekle
  delay(3000);
  
  digitalWrite(DIR_PIN, HIGH);
  delay(100);  
  
  
  digitalWrite(DIR_PIN, LOW); 
  delay(100);  
  
  // Motoru durdur
  analogWrite(PWM_PIN, 0);
  

  delay(1000); 
}
