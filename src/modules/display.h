#pragma ONCE

#ifndef Display_H
#define Display_H
#include "modules/canbus.h"

#define DISPLAY_CONTROL_ID 0x2A0240E
#define DISPLAY_MESSAGE_ID 0x1800008
#define MESSAGE_DELAY 40

class Display {
    public:
        static Display& getInstance() {
            static Display instance;
            return instance;
        }
        
        void turnOn();
        void turnOff();
        void clear();
        void clearLine(int line);
        void print(const char* text);
        void scrollText(int line, int start, int end, const char* text, int* textPos);
        void updateLine(int line, const char* text);
        void updatePartialLine(int line, int start, int end, const char* text);
    private:
        Display();
        char message[33] = "                                ";
        Canbus canbus;
};

#endif