#ifndef Menu_H
#define Menu_H

class Menu {
    public:
        char message[33];
        int textPos = 0;
        unsigned long lastMenuUpdate = 0;

        virtual void update() = 0;
    private:
};

#endif