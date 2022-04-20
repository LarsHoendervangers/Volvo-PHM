// Unavailable pins: 6,7,8,9,10,11
// Input only: 34, 35, 36, 39
#include <Wire.h>
#include "MCP23017.h"

#define SDA 21 // YELLOW
#define SCL 22 // BLUE

#define I2C_ONE 0x20
//#define SCAN_DELAY 50
#define SCAN_DELAY 25

#define KP_NO 'N'
#define KP_YES 'Y'
#define KP_POWER 'T'
#define KP_PREVIOUS 'P'
#define KP_NEXT 'F'
#define KP_CLEAR 'C'
#define KP_EMPTY '`'
#define BACKLIGHT_LEDS_PIN 33

#define ROWS 6
#define COLUMNS 3

char keys[ROWS][COLUMNS] = {
  { KP_YES,      KP_POWER, KP_NO},
  { KP_PREVIOUS, KP_CLEAR, KP_NEXT},
  { '1',         '2',      '3'},
  { '4',         '5',      '6'},
  { '7',         '8',      '9'},
  { '*',         '0',      '#'}
};

// Row pins are connected to port B
byte rowPins[ROWS] = { 8, 9, 10, 11, 12, 13 };
// Column pins are connected to port A
byte columnPins[COLUMNS] = { 0, 1, 2 };
MCP23017 MCP(0x20);

void setup() {
  Serial.begin(9600);
  Serial.println("Started");
  Wire.begin();
  MCP.begin();

  for (int col = 0; col < COLUMNS; col++) {
    MCP.pinMode(columnPins[col], INPUT_PULLUP);
  }

  for (int row = 0; row < ROWS; row++) {
    MCP.pinMode(rowPins[row], INPUT);
  }
}

char scanKeys() {
  char key = '\0';

  byte rowState = 0;
  int column = -1;
  for (int row = 0; row < ROWS; row++) {
    MCP.pinMode(rowPins[row], OUTPUT);
    MCP.digitalWrite(rowPins[row], LOW);
    delay(SCAN_DELAY);
    for (int col = 0; col < COLUMNS; col++) {
      if (MCP.digitalRead(columnPins[col]) == LOW) {
        bitSet(rowState, row);
        column = col;
      }
    }
    MCP.pinMode(rowPins[row], INPUT);
  }

  int foundCols = 0;
  for (int i = 0; i < 8; i++) {
    if (bitRead(rowState, i)) {
      foundCols++;
      if (foundCols == 1) key = keys[i][column];
    }
  }
  
  return key;
}

char getKey() {
  return scanKeys();
}

void loop() {
  char key = getKey();

  if (key) {
    Serial.print("Key ");
    Serial.print(key);
    Serial.print(" pressed.");
    Serial.println();
  }
}