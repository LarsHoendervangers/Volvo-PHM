#include "menus/mainmenu.h"
#include "modules/display.h"

MainMenu::MainMenu() {
    memcpy(message, "Geen telefoon           gevonden", 32);
}

void MainMenu::update() {
    if (firstUpdate) {
        // TODO: ONLY DO THIS IF STATEMENT ON A2DP CONNECT.
        for (int i = 0; i < 32; i++) message[i] = ' ';
        firstUpdate = false;
    }

    if (_phoneName != 0) { // Check if phoneName is not null
        if (strlen(_phoneName) > 11) {
            // for (int i = 0; i < 11; i++) {
            //     int pos = i + textPos;
            //     if (pos > strlen(_phoneName)) {
            //         message[i] = ' ';
            //         textPos = 0;
            //         break;
            //     } else {
            //         message[i] = _phoneName[pos];
            //     }
            // }
            
            
            for (int i = 0; i < 12; i++) {
                if (i + textPos < strlen(_phoneName)) {
                    message[i] = _phoneName[i + textPos];
                } else if (textPos > i - textPos - 7) {
                    // End of text reached, loop or reset to beginning
                    message[i] = 0x20; // Remove last char of string that is 'ghosting'.
                    textPos = -1;
                    break;
                }
            }
            textPos++;
            
            // Display::getInstance().scrollText(0, 0, 11, _phoneName, &textPos);
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
    } else {
        // TODO: Possibly this is not needed anymore.
        strcat(_phoneName, " ");
    }

    // if (_phoneName.length() <= 12) {
    //     for (int i = 0; i < 12; i++) {
    //         message[i] = _phoneName.charAt(i);
    //     }
    // }
    // shouldUpdate = true;
    // if (deviceName.length() < 12)
    //     Display::getInstance().updatePartialLine(0, 0, 11, deviceName.c_str());
}

void MainMenu::setServiceProvider(char* networkProvider) {
    _networkProvider = networkProvider;
    Serial.print("Set network provider: ");
    Serial.println(networkProvider);
    // for (int i = 16; i < 16 + 10; i++) {
    //     message[i] = _networkProvider[i - 16];
    //     Serial.print(message[i]);
    //     // message.setCharAt(i, _networkProvider.charAt(i - 16));
    // }
    memcpy(message + 16, networkProvider, 10);
    
    if (strlen(_networkProvider) > 10) {
        // message[16 + 11] = '.';
        // message[16 + 12] = '.';
        // message.setCharAt(16 + 11, '.');
        // message.setCharAt(16 + 12, '.');
    }
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