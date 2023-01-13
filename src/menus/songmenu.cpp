#include "menus/songmenu.h"
#include "modules/display.h"
#include "Arduino.h"

SongMenu::SongMenu() {

}

void SongMenu::update() {
    if (isPlaying) {
        // TODO: Update playtime here.
    }

    if (songTitle != 0) {
        if (songTitle.length() > 16) {
            // Display::getInstance().scrollText(0, 0, 16, songTitle.c_str(), &textPos);
        }
    }

    if (songArtist != 0) {
        
    }
}

void SongMenu::setSongTitle(String title) {
    if (title == 0) return;
    songTitle = title;
    if (songTitle.length() <= 16) {
        for (int i = 0; i < 16; i++) {
            message[i] = songTitle.charAt(i);
        }
    }
    // Display::getInstance().updateLine(0, title.c_str());
}

void SongMenu::setSongArtist(String artist) {
    if (artist == 0) return;
    songArtist = artist;
}

void SongMenu::setSongDuration(unsigned long duration) {
    songDuration = &duration;
}