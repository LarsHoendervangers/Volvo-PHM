#include <Arduino.h>
#include "modules/display.h"
#include "modules/keypad.h"
#include "modules/bluetooth.h"
#include "menus/mainmenu.h"
#include "menus/songmenu.h"

#include <freertos/semphr.h>

Display display = Display::getInstance();
Keypad keypad;

MainMenu mainMenu;
SongMenu songMenu;

Menu* menus[] = {
    &mainMenu,
    &songMenu
};

SemaphoreHandle_t menuIndexSemaphore;
int currentMenuIndex = 0;

void onA2DConnectionChanged() {
    if (xSemaphoreTake(menuIndexSemaphore, (TickType_t) 5) == pdTRUE) {
        mainMenu.connected();
        xSemaphoreGive(menuIndexSemaphore);
    }
}

void onPhoneDetailsChanged(PhoneDetails detail, phoneData* data) {
    if (xSemaphoreTake(menuIndexSemaphore, (TickType_t) 5) == pdTRUE) {
        if (detail & NAME) {
            mainMenu.setPhoneName(data->phoneName);
        }

        if (detail & SERVICE_PROVIDER) {
            mainMenu.setServiceProvider(data->serviceProvider);
            Serial.print(data->serviceProvider);
            Serial.println("Updating service provider");
        }

        if (detail & BATTERY) {
            mainMenu.setBatteryLevel(data->batteryLevel);
            Serial.print(data->batteryLevel);
            Serial.println("Updating battery level");
        }

        if (detail & ROAMING) {
            mainMenu.setRoaming(data->roaming);
            Serial.print(data->roaming);
            Serial.println("Updating roaming");
        }

        if (detail & SERVICE_STRENGTH) {
            mainMenu.setNetworkStrength(data->serviceStrength);
            Serial.print(data->serviceStrength);
            Serial.println("Updating service strength");
        }
        // currentMenuIndex = 1;
        xSemaphoreGive(menuIndexSemaphore);
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Started");
    
    menuIndexSemaphore = xSemaphoreCreateMutex();
    xSemaphoreGive(menuIndexSemaphore);

    keypad.begin();

    bt_registerA2DCallback(&onA2DConnectionChanged);
    bt_registerHFPCallback(&onPhoneDetailsChanged);
    bt_registerPhoneDetailsCallback(&onPhoneDetailsChanged);
    bt_begin();

    display.turnOn();
    display.clear();
    display.print(menus[currentMenuIndex]->message);
}

unsigned long lastMillis = 0;
void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastMillis > 1000) {
        lastMillis = currentMillis;
        if (xSemaphoreTake(menuIndexSemaphore, (TickType_t) 5) == pdTRUE) {
            menus[currentMenuIndex]->update();
            xSemaphoreGive(menuIndexSemaphore);
        }

        bt_update();
    }
    // char key = keypad.scanKeys();

    // if (key) {
    //     Serial.println(key);
    // }

    // delay(1000);
    // display.scrollText(0, 0, 11, text, &textPos);
}