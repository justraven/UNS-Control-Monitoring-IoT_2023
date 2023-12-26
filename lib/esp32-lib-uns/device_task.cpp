/*
* device_task.c
* Author : Muhammad Shofuwan Anwar
* Created : 2023-12-20 00:21:00
*/

#include "device_task.h"

void device_task_init (void) {
    dvtask_single_establish_connection();

    xTaskCreatePinnedToCore(dvtask_sensors, "dvtask_sensors", 4096, NULL, 1, NULL, app_cpu);
    xTaskCreatePinnedToCore(dvtask_time_keeping, "dvtask_time_keeping", 1024, NULL, 1, NULL, app_cpu);
    xTaskCreatePinnedToCore(dvtask_handle_client, "dvtask_handle_client", 4096, NULL, 2, NULL, app_cpu);
}

void dvtask_time_keeping (void *pvParameters) {
    time_keeping_t hwtime = {
        .hour = 0,
        .minute = 0,
        .second = 0
    };

    while (1) {
        hwtime.second++;

        if (hwtime.second >= 60) {
            hwtime.second = 0;
            hwtime.minute++;
        } else if (hwtime.minute >= 60) {
            hwtime.minute = 0;
            hwtime.hour++;
        } else if (hwtime.hour >= 24) {
            hwtime.hour = 0;
        }

        // update time keeping
        time_keeping_set(hwtime);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void dvtask_sensors (void *pvParameters) {
    sensors_data_t sensors_data;
    thingspeak_t thingspeak;

    time_keeping_t run_time = time_keeping_get();
    time_keeping_t elapsed_mins = {
        .hour = 0,
        .minute = 15,
        .second = 0
    };

    while (1) {
        run_time = time_keeping_get();

        if ( WiFi.status() == WL_CONNECTED 
             && time_keeping_multiple_mins(run_time, elapsed_mins)
             && (run_time.second == 0) ) {
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
        // remove the newlines from the end of the string
        hwdata.ssid.trim();
        hwdata.password.trim();

        WiFi.mode(WIFI_STA);
        WiFi.begin(hwdata.ssid.c_str(), hwdata.password.c_str());
        Serial.println("[INFO] Connecting to " + hwdata.ssid + "[" + String(hwdata.ssid.length()) + "]");
        Serial.println("[INFO] Password : " + hwdata.password + "[" + String(hwdata.password.length()) + "]");
        
        for (uint8_t x=0;x<max_timeout;x++) {
            Serial.print("[INFO] Timeout : " + String(max_timeout - x) + " ");

            for (uint8_t y=0;y<20;y++) {
                Serial.print("\b");
            }
            
            if (WiFi.status() == WL_CONNECTED) {
                Serial.println("\n[INFO] Connected to " + hwdata.ssid);
                hwdata.ipaddr = WiFi.localIP().toString();
                hwdata.macaddr = WiFi.macAddress();
                break;
            }

            delay(1000);
        }
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[ERROR] Failed to connect to " + hwdata.ssid);
        Serial.println("[INFO] Starting AP...");

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


