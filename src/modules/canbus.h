#pragma ONCE
#ifndef Canbus_H
#define Canbus_H
#include <Arduino.h>
#include "driver/twai.h"

class Canbus {
    public:
        Canbus();
        void send(unsigned long id, unsigned char *data);

    private:
        twai_message_t message;
};

#endif