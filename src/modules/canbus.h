#pragma ONCE
#ifndef Canbus_H
#define Canbus_H
#include <Arduino.h>
#include "driver/twai.h"

class Canbus {
    public:
        static Canbus& getInstance() {
            static Canbus canbusInstance;
            return canbusInstance;
        };
        Canbus();
        void send(unsigned long id, unsigned char *data);
        twai_message_t* read();

    private:

        twai_message_t txMsg;
        twai_message_t rxMsg;
};

#endif