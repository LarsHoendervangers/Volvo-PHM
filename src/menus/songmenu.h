#ifndef Songmenu_H
#define Songmenu_H

#include "menus/menu.h"
#include <Arduino.h>

class SongMenu : public Menu {
    public:
        SongMenu();
        void setSongTitle(char* title);
        void setSongArtist(char* artist);
        void setSongDuration(int duration);
        void setSongPlayback(int playback);
        void setPlaying(bool playing) { isPlaying = playing; };
        void update();
    private:
        bool shouldUpdate = 1;
        bool isPlaying = 0;
        char* songTitle;
        char* songArtist;
        int songDuration = 0;
        int songPlayback = 0;
};

#endif