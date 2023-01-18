#include "menus/songmenu.h"
#include "modules/display.h"
#include "Arduino.h"

SongMenu::SongMenu() {

}

void SongMenu::update() {
    if (shouldUpdate) {
        // for (int i = 0; i < 32; i++) message[i] = ' ';
        Display::getInstance().clear();
        shouldUpdate = false;
        Display::getInstance().print(message);
    }
}

void SongMenu::setSongTitle(char* title) {
    if (title == 0) return;
    songTitle = title;
    strncpy(message, title, 16);
    shouldUpdate = true;
}

void SongMenu::setSongArtist(char* artist) {
    if (artist == 0) return;
    songArtist = artist;
    strncpy(message + 16, artist, 16);
    shouldUpdate = true;
}

void SongMenu::setSongDuration(int duration) {
    songDuration = duration;
    shouldUpdate = true;
    Serial.print("Duration: ");
    Serial.println(duration);
}

void SongMenu::setSongPlayback(int playback) {
    songPlayback = playback;
    shouldUpdate = true;
    Serial.print("Playback: ");
    Serial.println(playback);
}