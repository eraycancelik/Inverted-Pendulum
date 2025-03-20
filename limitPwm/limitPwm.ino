#define RPWM 10      // PWM çıkışı olan pin (sağa yön)
#define LPWM 11      // PWM çıkışı olan pin (sola yön)
#define R_EN 8       // Sağa yön Enable pin
#define L_EN 9       // Sola yön Enable pin

#define runPWM 75    // Motor hızı

#include <ezButton.h>

#define RIGHT_SWITCH_PIN 6
#define LEFT_SWITCH_PIN 5

ezButton rightSwitch(RIGHT_SWITCH_PIN);
ezButton leftSwitch(LEFT_SWITCH_PIN);

bool direction = true; // true -> Sağa, false -> Sola

void setup() {
  Serial.begin(9600);

  rightSwitch.setDebounceTime(50);
  leftSwitch.setDebounceTime(50);

  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);
  delay(3000);
}

void loop() {
  rightSwitch.loop();
  leftSwitch.loop();

  Serial.print("Sağ Limit: ");
  Serial.print(rightSwitch.getState());
  Serial.print(" | Sol Limit: ");
  Serial.println(leftSwitch.getState());

  // Buton BIRAKILDIĞINDA yön değiştir
  if (rightSwitch.isReleased()) {
    direction = false; // Sola dönecek
    Serial.println("Sağ limit bırakıldı, SOLA dönüyor...");
  }

  if (leftSwitch.isReleased()) {
    direction = true; // Sağa dönecek
    Serial.println("Sol limit bırakıldı, SAĞA dönüyor...");
  }

  // Motor yönü değiştir
  if (direction) {
    analogWrite(RPWM, runPWM);
    analogWrite(LPWM, 0);
  } else {
    analogWrite(RPWM, 0);
    analogWrite(LPWM, runPWM);
  }

  delay(100);
}
