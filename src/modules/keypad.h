#ifndef Keypad_H
#define Keypad_H

#include "MCP23017.h"

#define KP_NO 'N'
#define KP_YES 'Y'
#define KP_POWER 'T'
#define KP_PREVIOUS 'P'
#define KP_NEXT 'F'
#define KP_CLEAR 'C'
#define KP_EMPTY '`'
#define BACKLIGHT_LEDS_PIN 33
#define SCAN_DELAY 25

#define ROWS 6
#define COLUMNS 3

class Keypad {
    public:
        Keypad();
        void begin();
        void enablePowerIndicator();
        void disablePowerIndicator();
        void enableBacklight();
        void disableBacklight();
        char scanKeys();

    private:
        char keys[ROWS][COLUMNS] = {
            { KP_YES,      KP_POWER, KP_NO},
            { KP_PREVIOUS, KP_CLEAR, KP_NEXT},
            { '1',         '2',      '3'},
            { '4',         '5',      '6'},
            { '7',         '8',      '9'},
            { '*',         '0',      '#'}
        };

        // Row pins are connected to port B
        unsigned char rowPins[ROWS] = { 8, 9, 10, 11, 12, 13 };
        // Column pins are connected to port A
        unsigned char columnPins[COLUMNS] = { 0, 1, 2 };
};

#endif