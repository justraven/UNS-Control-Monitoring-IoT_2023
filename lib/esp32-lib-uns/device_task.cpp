/*
* device_task.c
* Author : Muhammad Shofuwan Anwar
* Created : 2023-12-20 00:21:00
*/

#include "device_task.h"

static time_keeping_t __time_keeping = {0};

void device_task_init (void) {
    dvtask_single_establish_connection();

    xTaskCreatePinnedToCore(dvtask_sensors, "dvtask_sensors", 4096, NULL, 1, NULL, app_cpu);
    xTaskCreatePinnedToCore(dvtask_time_keeping, "dvtask_time_keeping", 1024, NULL, 1, NULL, app_cpu);
    xTaskCreatePinnedToCore(dvtask_handle_client, "dvtask_handle_client", 4096, NULL, 2, NULL, app_cpu);
}

void dvtask_time_keeping (void *pvParameters) {
    while (1) {
        __time_keeping.second++;

        if (__time_keeping.second >= 60) {
            __time_keeping.second = 0;
            __time_keeping.minute++;
        }
        if (__time_keeping.minute >= 60) {
            __time_keeping.minute = 0;
            __time_keeping.hour++;
        }
        if (__time_keeping.hour >= 24) {
            __time_keeping.hour = 0;
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void dvtask_sensors (void *pvParameters) {
    sensors_data_t sensors_data;
    thingspeak_t thingspeak;

    time_keeping_t elapsed_mins = {
        .hour = 0,
        .minute = 15,
        .second = 0
    };

    while (1) {
        if (WiFi.status() == WL_CONNECTED && time_keeping_multiple_mins(__time_keeping, elapsed_mins)) {
            sensors_data = sensors_sample();

            thingspeak = (thingspeak_t) {
                .api_key = (const char*)THINGSPEAK_API_KEY,
                .field = {
                    sensors_data.temperature,
                    sensors_data.humidity,
                    sensors_data.ambient_light,
                    0, 0, 0, 0, 0
                }
            };

            if (request_send_get_http(&thingspeak) == REQUEST_HTTP_OK) {
                Serial.println("[INFO] Data published to ThingSpeak.");
                status_blink(STATUS_DATA_PUBLISHED, 1, STATUS_DEFAULT_DELAY_MS);
            } else {
                Serial.println("[ERROR] Data not published to ThingSpeak.");
                status_blink(STATUS_DATA_ERROR, 1, STATUS_DEFAULT_DELAY_MS);
            }
        }

        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

void dvtask_handle_client (void *pvParameters) {
    while (1) {
        server_handle_client();
    }
}

void dvtask_single_establish_connection (void) {
    connections_flags_t connections_flags = connections_init();
    if (connections_flags != CONNECTIONS_SPIFFS_OK) {
        status_blink(STATUS_WLAN_ERROR, 5, STATUS_DEFAULT_DELAY_MS);
        Serial.println("[ERROR] SPIFFS initialization failed.");
    }

    /*
     * Try to connect wifi
     * if failed, start AP
     */
    connections_hwdata_t hwdata;
    connections_flags = connections_read_config(&hwdata);

    const uint8_t max_timeout = 10;
    if (connections_flags == CONNECTIONS_SPIFFS_OK) {
        WiFi.begin(hwdata.ssid.c_str(), hwdata.password.c_str());
        Serial.println("[INFO] Connecting to " + hwdata.ssid);
        
        for (uint8_t x=0;x<max_timeout;x++) {
            Serial.print("[INFO] Timeout : " + String(max_timeout - x) + " ");

            for (uint8_t y=0;y<20;y++) {
                Serial.print("\b");
            }
            
            if (WiFi.status() == WL_CONNECTED) {
                hwdata.ipaddr = WiFi.localIP().toString();
                hwdata.macaddr = WiFi.macAddress();
                break;
            }

            delay(1000);
        }
        Serial.println("");

        Serial.println("[INFO] Connected to " + hwdata.ssid);
    } else {
        connections_flags = connections_start_ap(&hwdata);
        if (connections_flags != CONNECTIONS_SUCCESS) {
            status_blink(STATUS_WLAN_ERROR, 5, STATUS_DEFAULT_DELAY_MS);
            Serial.println("[ERROR] AP initialization failed.");
            for (uint8_t x=0;x<(max_timeout/2);x++) {
                Serial.print("[INFO] Restarting in " + String(max_timeout - (x*2)) + " ");
                
                for (uint8_t y=0;y<25;y++) {
                    Serial.print("\b");
                }
                
                delay(1000);
            }
            Serial.println("");

            ESP.restart();
        }
    }

    // run server
    server_init();

    Serial.println("[INFO] Server initialized.");
    Serial.println("[INFO] IP address: " + hwdata.ipaddr);
    Serial.println("[INFO] MAC address: " + hwdata.macaddr);
}

uint8_t time_keeping_compare (time_keeping_t first, time_keeping_t second) {
    if (first.hour == second.hour && first.minute == second.minute && first.second == second.second) {
        return 1;
    } else {
        return 0;
    }
}

uint8_t time_keeping_multiple_mins (time_keeping_t first, time_keeping_t multiple) {
    if ((first.minute % multiple.minute) == 0) {
        return 1;
    } else {
        return 0;
    }
}


