#ifndef Songmenu_H
#define Songmenu_H

#include "menus/menu.h"
#include <Arduino.h>

class SongMenu : public Menu {
    public:
        SongMenu();
        void setSongTitle(String title);
        void setSongArtist(String artist);
        void setSongDuration(unsigned long duration);
        void setPlaying(bool playing) { isPlaying = playing; };
        void update();
    private:
        bool isPlaying = 0;
        String songTitle;
        String songArtist;
        unsigned long int* songDuration;
};

#endif