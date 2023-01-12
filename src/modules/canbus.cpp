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
    message.identifier = id;
    message.extd = 1;
    message.data_length_code = 8;

    for (int i = 0; i < 8; i++) {
        message.data[i] = data[i];
        // Serial.print("0x");
        // Serial.print(message.data[i], HEX);
        // Serial.print(" ");
    }
    // Serial.print(" Send status: ");
    // Serial.println(twai_transmit(&message, pdMS_TO_TICKS(1000)));
    twai_transmit(&message, pdMS_TO_TICKS(1000));
}