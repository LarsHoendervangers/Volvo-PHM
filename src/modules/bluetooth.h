#ifndef Bluetooth_H
#define Bluetooth_H

#include "BluetoothA2DPSink.h"
#include "esp_hf_client_api.h"
#include "Arduino.h"

typedef struct phoneData {
    char phoneName[32];
    char serviceProvider[32];
    int batteryLevel = -1;
    int serviceStrength = -1;
    bool roaming = 0;
} phoneData;

typedef struct musicData {
    char title[128];
    char artist[64];
    int duration = -1;
} musicData;

enum PhoneDetails {
    NAME = 1, 
    ROAMING = 2, 
    BATTERY = 4, 
    SERVICE_PROVIDER = 8, 
    SERVICE_STRENGTH = 16
};

inline PhoneDetails operator|(PhoneDetails a, PhoneDetails b) {
    return static_cast<PhoneDetails>(static_cast<int>(a) | static_cast<int>(b));
};

enum MusicDetails {
    TITLE = 1,
    ARTIST = 2,
    DURATION = 4
};

inline MusicDetails operator|(MusicDetails a, MusicDetails b) {
    return static_cast<MusicDetails>(static_cast<int>(a) | static_cast<int>(b));
};

enum MusicControl {
    NEXT_TRACK, PREVIOUS_TRACK, PAUSE, PLAY
};

void bt_begin();
void bt_update();
void queryPhoneDetails();

void bt_a2d_cb(uint8_t event, const uint8_t *param);
void bt_a2d_conn_state_changed(esp_a2d_connection_state_t state, void *ptr);
void bt_registerA2DConnectionCallback(void(*)(esp_a2d_connection_state_t));
void bt_registerA2DSongCallback(void(*)(MusicDetails, musicData*));

void bt_registerPhoneDetailsCallback(void(*)(PhoneDetails, phoneData*));

void bt_hfp_cb(esp_hf_client_cb_event_t event, esp_hf_client_cb_param_t *param);
void bt_registerHFPCallback(void(*)(PhoneDetails, phoneData*));

void bt_musicControl(MusicControl);
#endif