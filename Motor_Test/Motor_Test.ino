#define RPWM 10      // PWM çıkışı olan pin (sağa yön)
#define LPWM 11      // PWM çıkışı olan pin (sola yön)
#define R_EN 8       // Sağa yön Enable pin
#define L_EN 9       // Sola yön Enable pin

#define runPWM 100

void setup() {
  // Pinleri çıkış olarak ayarla
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  
  // Enable pinlerini HER ZAMAN HIGH yap (eğer 5V'a bağlı değillerse)
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  // Motoru başta durdur
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);
  delay(3000); 
}

void loop() {
  analogWrite(RPWM, runPWM); 
  analogWrite(LPWM, 0);     

  delay(500); 
  
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);

  delay(200);
  
  analogWrite(RPWM, 0); 
  analogWrite(LPWM, runPWM);     

  delay(500); 
  
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);

  delay(2300);
 
}
