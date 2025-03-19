#define LED_PIN 13  // Arduino Nano'nun dahili LED pini

void setup() {
  Serial.begin(9600);  // Seri haberleşmeyi başlat
  pinMode(LED_PIN, OUTPUT); // LED pinini çıkış olarak ayarla
}

void loop() {
  if (Serial.available() > 0) { // Eğer seri porttan veri gelirse
    int blinkCount = Serial.parseInt(); // Girilen sayıyı oku
    
    if (blinkCount > 0) { // Geçerli bir sayı girildiyse
      Serial.print("Blinking ");
      Serial.print(blinkCount);
      Serial.println(" times...");

      for (int i = 0; i < blinkCount; i++) {
        digitalWrite(LED_PIN, HIGH); // LED'i yak
        delay(200); // 200 ms bekle
        digitalWrite(LED_PIN, LOW);  // LED'i söndür
        delay(200);
      }
    }
    Serial.println("Ready for next input...");
  }
}
