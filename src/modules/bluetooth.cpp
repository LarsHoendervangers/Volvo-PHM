#include "modules/bluetooth.h"

const char deviceName[] = "My S60";
phoneData sourceData;
musicData songData;
BluetoothA2DPSink a2dp_sink;
void(*phoneDataCallback)(PhoneDetails, phoneData*);
void(*hfpCallback)(PhoneDetails, phoneData*);
void(*a2dpConnCallback)(esp_a2d_connection_state_t);
void(*a2dpMusicCallback)(MusicDetails, musicData*);

esp_avrc_rn_param_t rnParams;
esp_avrc_rn_rsp_t rnResponse;

void avrc_tg_callback(esp_avrc_tg_cb_t event, esp_avrc_tg_cb_param_t* param) {
    Serial.printf("AVRC TG callback: attribute id 0x%x, %s\n", event, param);
}

void avrc_onAudioStateChangedCB(esp_a2d_audio_state_t state, void* param) {
    Serial.printf("AVRC audio state changed callback: %s\n", a2dp_sink.to_str(state));
}

void bt_begin() {
    // A2D
    a2dp_sink.set_avrc_metadata_attribute_mask(ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_PLAYING_TIME | ESP_AVRC_MD_ATTR_ARTIST);
    a2dp_sink.set_avrc_metadata_callback(bt_a2d_cb);
    i2s_pin_config_t const i2sportConfig  = {
        .bck_io_num = 27,
        .ws_io_num = 26,
        .data_out_num = 25,
        .data_in_num = I2S_PIN_NO_CHANGE
    };  
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 44100, // updated automatically by A2DP
        .bits_per_sample = (i2s_bits_per_sample_t)32,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = 0, // default interrupt priority
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = true,
        .tx_desc_auto_clear = true // avoiding noise in case of data unavailability
    };
    a2dp_sink.set_i2s_config(i2s_config);
    a2dp_sink.set_pin_config(i2sportConfig);
    a2dp_sink.set_on_connection_state_changed(bt_a2d_conn_state_changed);

    a2dp_sink.start("My S60");
    // HFP
    esp_hf_client_register_callback(bt_hfp_cb);
    esp_hf_client_init();

    a2dp_sink.set_on_audio_state_changed(avrc_onAudioStateChangedCB);
    // esp_avrc_tg_register_callback(avrc_tg_callback);
}

int seconds = 0;
void bt_update() {
    seconds++;
    if (seconds > 120) {
        queryPhoneDetails();
        seconds = 0;
    }
}

void queryPhoneDetails() {
    const char* sourceName = a2dp_sink.get_connected_source_name();
    if (sourceData.phoneName != sourceName) {
        // TODO: Check if strlen needs to check sourceName or sourceData.phoneName
        memcpy(sourceData.phoneName, sourceName, strlen(sourceName));
        phoneDataCallback(NAME, &sourceData);
    }
    esp_hf_client_query_current_operator_name();
    esp_hf_client_retrieve_subscriber_info();
}

// A2DP
void bt_a2d_cb(uint8_t event, const uint8_t *param) {
    Serial.printf("A2D callback: attribute id 0x%x, %s\n", event, param);
    MusicDetails details;
    if (event == ESP_AVRC_MD_ATTR_TITLE) {
        strcpy(songData.title, (const char*)param);
        details = details | TITLE;
    }

    if (event == ESP_AVRC_MD_ATTR_ARTIST) {
        strcpy(songData.artist, (const char*)param);
        details = details | ARTIST;
    }

    if (event == ESP_AVRC_MD_ATTR_PLAYING_TIME) {
        songData.duration = atoi((const char*)param);
        details = details | DURATION;
    }

    if (event == ESP_AVRC_PLAYBACK_PLAYING) {
        songData.playback = (int)param;
        details = details | PLAYBACK;
    }

    if (details != 0) {
        a2dpMusicCallback(details, &songData);
    }
}

void bt_a2d_conn_state_changed(esp_a2d_connection_state_t state, void *ptr) {
    if (state == ESP_A2D_CONNECTION_STATE_CONNECTED) {
        seconds = 118;
        esp_avrc_tg_send_rn_rsp(ESP_AVRC_RN_PLAY_POS_CHANGED, rnResponse, &rnParams);
    }
    a2dpConnCallback(state);
}

void bt_registerPhoneDetailsCallback(void(callback)(PhoneDetails, phoneData*)) {
    phoneDataCallback = callback;
}


// HFP
void bt_hfp_cb(esp_hf_client_cb_event_t event, esp_hf_client_cb_param_t *param) {
    PhoneDetails details;
    if (event == ESP_HF_CLIENT_CIND_SERVICE_AVAILABILITY_EVT &&
        sourceData.serviceStrength != param->service_availability.status) {
        sourceData.serviceStrength = param->service_availability.status;
        details = details | SERVICE_STRENGTH;
    }

    if (event == ESP_HF_CLIENT_CIND_BATTERY_LEVEL_EVT &&
        sourceData.batteryLevel != param->battery_level.value) {
        sourceData.batteryLevel = param->battery_level.value;
        details = details | BATTERY;
    }

    if (event == ESP_HF_CLIENT_COPS_CURRENT_OPERATOR_EVT &&
        sourceData.serviceProvider != param->cops.name) {
        memcpy(sourceData.serviceProvider, param->cops.name, strlen(param->cops.name));
        details = details | SERVICE_PROVIDER;
    }

    if (event == ESP_HF_CLIENT_CIND_ROAMING_STATUS_EVT &&
        sourceData.roaming != param->roaming.status) {
        sourceData.roaming = param->roaming.status;
        details = details | ROAMING;
    }

    if (details != 0) {
        phoneDataCallback(details, &sourceData);
    }
}

void bt_registerA2DConnectionCallback(void(*callback)(esp_a2d_connection_state_t)) {
    a2dpConnCallback = callback;
}

void bt_registerA2DSongCallback(void(*callback)(MusicDetails, musicData*)) {
    a2dpMusicCallback = callback;
}

void bt_registerHFPCallback(void(callback)(PhoneDetails, phoneData*)) {
    hfpCallback = callback;
}

void bt_musicControl(MusicControl control) {
    switch (control) {
        case NEXT_TRACK:
            a2dp_sink.next();
            break;
        case PREVIOUS_TRACK:
            a2dp_sink.previous();
            break;
        case PAUSE:
            a2dp_sink.pause();
            break;
        case PLAY:
            a2dp_sink.play();
            break;
    }
}