#ifndef Mainmenu_H
#define Mainmenu_H

#include "menus/menu.h"
#include "Arduino.h"

class MainMenu : public Menu {
    public:
        MainMenu();
        void setPhoneName(char* phoneName);
        void setServiceProvider(char* networkProvider);
        void setBatteryLevel(int level);
        void setRoaming(bool roaming);
        void setNetworkStrength(int strength);
        void connected() { firstUpdate = true; }

        void update();
    private:
        char* _phoneName;
        char* _networkProvider;
        int _batteryLevel = -1;
        bool _roaming = -1;
        int _networkStrength = -1;

        bool shouldUpdate = false;
        bool firstUpdate = false;
};

#endif