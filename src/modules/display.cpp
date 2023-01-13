#include <Arduino.h>
#include "modules/display.h"

Display::Display() {
    
}

void Display::turnOn() {
    unsigned char openDim[8] = {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35};
    canbus.send(DISPLAY_CONTROL_ID, openDim);
    delay(MESSAGE_DELAY);
    openDim[7] = 0x31;
    canbus.send(DISPLAY_CONTROL_ID, openDim);
}

void Display::turnOff() {

}

void Display::clear() {
    unsigned char clearDim[8] = {0xE1, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    canbus.send(DISPLAY_CONTROL_ID, clearDim);
}

void Display::clearLine(int line) {
    for (int i = 16 * line; i < (16 * (line + 1)); i++) {
        message[i] = 0x20;
    }
    print(message);
}

// /**
//  * @brief Scrolls a string between a start and end index. Each time this method is called it will progress the scroll.
//  * 
//  * @param line The line on the display on which this text will scroll.
//  * @param start Start position on the display.
//  * @param end End position on the display.
//  * @param text The text that has to be scrolled through (can be longer than the display width).
//  * @param textPos The index at where to start printing the chars in the 'text' array.
//  */
// void Display::scrollText(int line, int start, int end, const char* text, int* textPos) {
//     for (int i = start; i < end; i++) {
//         if (i + *textPos < strlen(text)) {
//             message[(line * 16) + i] = text[i + *textPos];
//         } else {
//             // End of text reached, loop or reset to beginning
//             message[(line * 16) + i] = 0x20; // Remove last char of string that is 'ghosting'.
//             *textPos = -1;
//             break;
//         }
//     }
//     *textPos += 1;
//     print(message);
// }

// /**
//  * @brief Updates a single line on the display.
//  * 
//  * @param line 0: upper line, 1: bottom line.
//  * @param text Text to be displayed. Max 16 characters.
//  */
// void Display::updateLine(int line, const char* text) {
//     for (int i = 16 * line; i < (16 * (line + 1)) - 1; i++) {
//         message[i] = text[i - (16 * line)];
//     }
//     print(message);
// }

// void Display::updatePartialLine(int line, int start, int end, const char* text) {
//     for (int i = start + (16 * line); i < end + (16 * line); i++) {
//         message[i] = text[i - (16 * line)];
//     }
//     print(message);
// }

/**
 * @brief Displays up to 32 characters on the display of a volvo DIM.
 * 
 * Heavily inspired by https://github.com/martonn98/VolvoP2_CAN/blob/main/ArduinoPoC/String2DIM/String2DIM.ino
 * @param text Text to be displayed. Max 32 characters.
 */
void Display::print(const char* text) {
    char str[32] = {0};
    for (int i = 0; i < 32; i++) {
        str[i] = 0x20;
    }
    memcpy(str, text, strlen(text));

    uint8_t d[] = {0xA7, 0x00, str[0], str[1], str[2], str[3], str[4], str[5]};
    canbus.send(DISPLAY_MESSAGE_ID, d);
    delay(MESSAGE_DELAY);
    uint8_t d2[] = {0x21, str[6], str[7], str[8], str[9], str[10], str[11], str[12]};
    canbus.send(DISPLAY_MESSAGE_ID, d2);
    delay(MESSAGE_DELAY);
    uint8_t d3[] = {0x22, str[13], str[14], str[15], str[16], str[17], str[18], str[19]};
    canbus.send(DISPLAY_MESSAGE_ID, d3);
    delay(MESSAGE_DELAY);
    uint8_t d4[] = {0x23, str[20], str[21], str[22], str[23], str[24], str[25], str[26]};
    canbus.send(DISPLAY_MESSAGE_ID, d4);
    delay(MESSAGE_DELAY);
    uint8_t d5[] = {0x65, str[27], str[28], str[29], str[30], str[31], 0x00, 0x00};
    canbus.send(DISPLAY_MESSAGE_ID, d5);
    delay(MESSAGE_DELAY);
}