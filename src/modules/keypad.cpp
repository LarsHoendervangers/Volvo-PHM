#include <Arduino.h>
#include "modules/keypad.h"

// 0x20 is the address of the IO expander
MCP23017 keypadMcp(0x20);
Keypad::Keypad() {

}

void Keypad::begin() {
    Wire.begin();
    keypadMcp.begin();

    for (int col = 0; col < COLUMNS; col++) {
        keypadMcp.pinMode(columnPins[col], INPUT_PULLUP);
    }

    for (int row = 0; row < ROWS; row++) {
        keypadMcp.pinMode(rowPins[row], INPUT);
    }
}

void Keypad::enablePowerIndicator() {

}

void Keypad::disablePowerIndicator() {

}

void Keypad::enableBacklight() {

}

void Keypad::disableBacklight() {

}

// Scans every row and column to find the key that is pressed.
char Keypad::scanKeys() {
  char key = '\0'; // Set 'key' to null.

  // Rowstate holds the true/false value of each row.
  // The pressed column will flip a bit on position 'row' to one.
  int rowState = 0;
  int column = -1;
  for (int row = 0; row < ROWS; row++) {
    keypadMcp.pinMode(rowPins[row], OUTPUT);
    keypadMcp.digitalWrite(rowPins[row], LOW);
    delay(SCAN_DELAY);
    for (int col = 0; col < COLUMNS; col++) {
      if (keypadMcp.digitalRead(columnPins[col]) == LOW) {
        bitSet(rowState, row);
        column = col;
      }
    }
    // Set row pins back the input to get more accurate readings.
    keypadMcp.pinMode(rowPins[row], INPUT);
  }

  int foundCols = 0;
  for (int i = 0; i < 8; i++) {
    if (bitRead(rowState, i)) {
      foundCols++;
      // If there is only one active column, return the pressed key.
      // If a bad reading occurs multiple columns will be found.
      // So that is why this is a '==' check.
      if (foundCols == 1) key = keys[i][column];
    }
  }
  
  return key;
}