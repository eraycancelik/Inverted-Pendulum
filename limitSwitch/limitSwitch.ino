#include <ezButton.h>

#define LIMIT_SWITCH_LEFT 0  // Birinci limit switch pini
#define LIMIT_SWITCH_RIGHT 6  // İkinci limit switch pini

ezButton buttonLEFT(LIMIT_SWITCH_LEFT);
ezButton buttonRIGHT(LIMIT_SWITCH_RIGHT);

void setup() {
    Serial.begin(115200);
    buttonLEFT.setDebounceTime(50); // Buton titreşimini önlemek için
    buttonRIGHT.setDebounceTime(50);
}

void loop() {
    buttonLEFT.loop();
    buttonRIGHT.loop();
    
    if (buttonLEFT.isReleased()) {
        Serial.println("Sağ limitSwitch'e basıldı");
        delay(200); 
    }
    if (buttonRIGHT.isReleased()){
        Serial.println("Sağ limitSwitch'e basıldı");
        delay(200);
    }
}
