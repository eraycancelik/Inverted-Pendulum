#include <ezButton.h>

ezButton rightSwitch(6);  // Sağ limit switch, pin 6
ezButton leftSwitch(5);   // Sol limit switch, pin 5

int lastRightState = LOW; // Önceki sağ switch durumu
int lastLeftState = LOW;  // Önceki sol switch durumu

void setup() {
  Serial.begin(9600);
  rightSwitch.setDebounceTime(50); // Debounce süresi 50 ms
  leftSwitch.setDebounceTime(50);
}

void loop() {
  rightSwitch.loop();
  leftSwitch.loop();
  
  int rightState = rightSwitch.getState();
  int leftState = leftSwitch.getState();

  // Eğer sağ switch durumu değiştiyse ekrana yazdır
  if (rightState != lastRightState) {
    if (rightState == HIGH)
      Serial.println("Right limit switch: TOUCHED");
    else
      Serial.println("Right limit switch: UNTOUCHED");
    
    lastRightState = rightState; // Önceki durumu güncelle
  }

  // Eğer sol switch durumu değiştiyse ekrana yazdır
  if (leftState != lastLeftState) {
    if (leftState == HIGH)
      Serial.println("Left limit switch: TOUCHED");
    else
      Serial.println("Left limit switch: UNTOUCHED");

    lastLeftState = leftState; // Önceki durumu güncelle
  }
}
