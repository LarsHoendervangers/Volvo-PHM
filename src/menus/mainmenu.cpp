#include "menus/mainmenu.h"
#include "modules/display.h"

MainMenu::MainMenu() {
    memcpy(message, "Geen telefoon           gevonden", 32);
}

void MainMenu::update() {
    if (firstUpdate) {
        for (int i = 0; i < 32; i++) message[i] = ' ';
        Display::getInstance().clear();
        firstUpdate = false;
        Serial.println("Clearing display for inital main menu update.");
    }

    if (_phoneName != 0) { // Check if phoneName is not null
        if (strlen(_phoneName) > 11) {      
            // Serial.printf("TextPos: %d, strlen - 11: %d\n", textPos, strlen(_phoneName) - 11);     
            for (int i = 0; i < 12; i++) {
                if (textPos > (strlen(_phoneName) - 11) - 1) {
                    // End of text reached, loop or reset to beginning
                    // message[i] = 0x20; // Remove last char of string that is 'ghosting'.
                    textPos = -1;
                    break;
                } else if (i + textPos < strlen(_phoneName)) {
                    message[i] = _phoneName[i + textPos];
                } else {
                    Serial.println("phone name else");
                }
            }
            textPos++;
        }
    }
    
    if (shouldUpdate || (_phoneName && strlen(_phoneName) > 11)) {
        Display::getInstance().print(message);
        if (shouldUpdate) shouldUpdate = false;
    }
}

void MainMenu::setPhoneName(char* deviceName) {
    _phoneName = deviceName;
    if (strlen(deviceName) < 12) {
        memcpy(message, deviceName, strlen(deviceName));
        shouldUpdate = true;
    }
}

void MainMenu::setServiceProvider(char* networkProvider) {
    _networkProvider = networkProvider;
    Serial.print("Set network provider: ");
    Serial.println(networkProvider);
    memcpy(message + 16, networkProvider, 10);
    shouldUpdate = true;
}

void MainMenu::setBatteryLevel(int level) {
    _batteryLevel = level;
    Serial.printf("Setting bat. level: %x\n", level);
    if (level == 5) {
        message[29] = 0xFF;
        message[30] = 0xFF;
        message[31] = 0xFF;
    } else if (level == 4) {
        message[29] = 0xFF;
        message[30] = 0xFF;
        message[31] = 0x0F;
    } else if (level == 3) {
        message[29] = 0xFF;
        message[30] = 0x0F;
        message[31] = '-';
    } else if (level == 2) {
        message[29] = 0x0F;
        message[30] = ']';
        message[31] = '-';
    } else if (level == 1) {
        message[29] = '[';
        message[30] = '-';
        message[31] = '-';
    } else if (level == 0) {
        message[29] = '-';
        message[30] = '-';
        message[31] = '-';
    }

    shouldUpdate = true;
}

void MainMenu::setRoaming(bool roaming) {
    _roaming = roaming;
    if (roaming) message[13] = 0x01;
    else message[13] = ' ';
    shouldUpdate = true;
}

void MainMenu::setNetworkStrength(int strength) {
    _networkStrength = strength;

    if (strength >= 4) {
        message[14] = 0x06;
        message[15] = 0x08;
    } else if (strength == 3) {
        message[14] = 0x06;
        message[15] = 0x07;
    } else if (strength == 2) {
        message[14] = 0x06;
        message[15] = ' ';
    } else if (strength == 1) {
        message[14] = 0x05;
        message[15] = ' ';
    } else if (strength <= 0) {
        message[14] = '-';
        message[15] = '-';
    }

    shouldUpdate = true;
}