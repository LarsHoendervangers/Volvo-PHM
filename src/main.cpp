#include <Arduino.h>
#include "modules/canbus.h"
#include "modules/display.h"
#include "modules/keypad.h"
#include "modules/bluetooth.h"
#include "menus/mainmenu.h"
#include "menus/songmenu.h"

#include <freertos/semphr.h>

#define MAIN_MENU_INDEX 0
#define SONG_MENU_INDEX 1

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

void onA2DConnectionChanged(esp_a2d_connection_state_t state) {
    if (xSemaphoreTake(menuIndexSemaphore, (TickType_t) 5) == pdTRUE) {
        if (state == ESP_A2D_CONNECTION_STATE_CONNECTED) {
            mainMenu.connected();
        } else if (state == ESP_A2D_CONNECTION_STATE_CONNECTING) {
            // Display::getInstance().print("Verbinden met           telefoon");
            Display::getInstance().print("Zoeken naar          telefoon...");
        }
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
        xSemaphoreGive(menuIndexSemaphore);
    }
}

void onSongUpdated(MusicDetails details, musicData* data) {

}

void setup() {
    Serial.begin(115200);
    Serial.println("Started");
    
    menuIndexSemaphore = xSemaphoreCreateMutex();
    xSemaphoreGive(menuIndexSemaphore);

    keypad.begin();

    bt_registerA2DSongCallback(&onSongUpdated);
    bt_registerA2DConnectionCallback(&onA2DConnectionChanged);
    bt_registerHFPCallback(&onPhoneDetailsChanged);
    bt_registerPhoneDetailsCallback(&onPhoneDetailsChanged);
    bt_begin();

    display.turnOn();
    display.clear();
    display.print(menus[currentMenuIndex]->message);
}

void setCurrentMenuIndex(int index) {
    if (index > sizeof(menus)) index = 0;
    else if (index < 0) index = sizeof(menus);
    if (xSemaphoreTake(menuIndexSemaphore, (TickType_t) 5) == pdTRUE) {
        if (index > 0) currentMenuIndex++;
        else if (index < 0) currentMenuIndex--;

        if (currentMenuIndex > sizeof(menus) - 1) currentMenuIndex = 0;
        else if (currentMenuIndex < sizeof(menus) - 1) currentMenuIndex = sizeof(menus) - 1;

        currentMenuIndex = index;
        xSemaphoreGive(menuIndexSemaphore);
    }
}

void onKeyPressed(char key) {
    if (!key) return;

    if (key == KP_NEXT) {
        setCurrentMenuIndex(1);
    } else if (key == KP_PREVIOUS) {
        setCurrentMenuIndex(-1);
    }
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
    onKeyPressed(keypad.scanKeys());
    // Canbus::getInstance().read();
}