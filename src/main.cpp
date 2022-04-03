#include <Arduino.h>
#include <Keypad.h>

#define KP_NO 'N'
#define KP_YES 'Y'
#define KP_POWER 'T'
#define KP_PREVIOUS 'P'
#define KP_NEXT 'F'
#define KP_CLEAR 'C'
#define KP_EMPTY '`'

#define BACKLIGHT_LEDS_PIN 33

const byte rows = 3;
const byte columns = 7;
char keys[rows][columns] = {
  {KP_EMPTY, KP_YES,   KP_PREVIOUS, '1', '4', '7', '*'},
  {KP_POWER, KP_CLEAR,    '2', '5', '8', '0', KP_EMPTY},
  {          KP_NO,    KP_NEXT,     '3', '6', '9', '#', KP_EMPTY}  
  // {KP_EMPTY, KP_YES,   KP_PREVIOUS, '1', '4', '7', '*'},
  // {KP_EMPTY, KP_POWER, KP_CLEAR,    '2', '5', '8', '0' },
  // {KP_EMPTY, KP_NO,    KP_NEXT,     '3', '6', '9', '#' }
};

byte rowPins[rows] = { 18, 19, 21 };
byte columnPins[columns] = { 25, 22, 26, 23, 27, 13, 0 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);

// Unavailable pins: 6,7,8,9,10,11
// Input only: 34, 35, 36, 39

void setup() {
  Serial.begin(9600);
  Serial.println("Version 1.003");

  keypad.setDebounceTime(50);

  pinMode(BACKLIGHT_LEDS_PIN, OUTPUT);
  digitalWrite(BACKLIGHT_LEDS_PIN, HIGH);
}

void loop() {
  char key = keypad.getKey();

  if (key && key) {
    Serial.println(key);
  }

  // if (keypad.getKeys()) {
  //   String msg = "";
  //   for (int i = 0; i < LIST_MAX; i++) {
  //     if (keypad.key[i].stateChanged) {
  //         switch (keypad.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
  //             case PRESSED:
  //             msg = " PRESSED.";
  //         break;
  //             case HOLD:
  //             msg = " HOLD.";
  //         break;
  //             case RELEASED:
  //             msg = " RELEASED.";
  //         break;
  //             case IDLE:
  //             msg = " IDLE.";
  //         }
  //         Serial.print("Key ");
  //         Serial.print(keypad.key[i].kchar);
  //         Serial.println(msg);
  //     }
  //   }
  //   Serial.println(" ");
  // }
}