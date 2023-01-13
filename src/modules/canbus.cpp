#include "modules/canbus.h"

Canbus::Canbus() {
    //Initialize configuration structures using macro initializers
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_32, GPIO_NUM_33, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_125KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    //Install TWAI driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        printf("Driver installed\n");
    } else {
        printf("Failed to install driver\n");
        return;
    }

    //Start TWAI driver
    if (twai_start() == ESP_OK) {
        printf("Driver started\n");
    } else {
        printf("Failed to start driver\n");
    }
}

void Canbus::send(unsigned long id, unsigned char *data) {
    txMsg.identifier = id;
    txMsg.extd = 1;
    txMsg.data_length_code = 8;

    for (int i = 0; i < 8; i++) {
        txMsg.data[i] = data[i];
    }
    twai_transmit(&txMsg, pdMS_TO_TICKS(1000));
}

twai_message_t* Canbus::read() {
    if (twai_receive(&rxMsg, pdMS_TO_TICKS(10000)) == ESP_OK) {
        if (rxMsg.extd) {
            printf("%x:", rxMsg.identifier & 0x1FFFFFFF);
            if (!(rxMsg.rtr)) {
                for (int i = 0; i < rxMsg.data_length_code; i++) {
                    printf(" %x", rxMsg.data[i]);
                }
                printf("\n");
                return &rxMsg;
            }
        }
    }
    return NULL;
}