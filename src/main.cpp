#include <Arduino.h>
#include "modules/RN52.h"

RN52 rn52;

void setup() {
    Serial.begin(9600);
    rn52.begin();
    delay(400);
    rn52.requestAGBatteryLevel();
}

bool hasPrinted = false;
void loop() {
    rn52.update();
    if (millis() >= 3000 && !hasPrinted) {
        hasPrinted = true;
        Serial.println(rn52.getAG().connectedMAC);
        Serial.println(rn52.getAG().batteryLevel);
    }
}