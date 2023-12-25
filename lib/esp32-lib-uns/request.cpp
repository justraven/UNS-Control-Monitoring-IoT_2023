/*
 * request.cpp
 * Author : Muhammad Shofuwan Anwar
 * Created : 2023-12-20 00:21:00
 */

#include "request.h"

const char *thingspeak_key_api = THINGSPEAK_API_KEY;
const char *thingspeak_url = "http://api.thingspeak.com/update?api_key=";

request_flags_t request_send_get_http (thingspeak_t *thingspeak) {
    if (WiFi.status() == WL_CONNECTED) {
        const String field[] = {
            "&field1=", "&field2=", "&field3=", "&field4=",
            "&field5=", "&field6=", "&field7=", "&field8="
        };
        
        String url = thingspeak_url + String(thingspeak->api_key);
        for (uint8_t x=0;x<8;x++) {
            if (thingspeak->field[x] > 0) {
                url += field[x] + String(thingspeak->field[x]);
            }
        }

        HTTPClient http;
        http.begin(url);
        int http_code = http.GET();

        if (http_code == HTTP_CODE_OK) {
            return REQUEST_HTTP_OK;
        }
    }

    return REQUEST_HTTP_ERR;
}


