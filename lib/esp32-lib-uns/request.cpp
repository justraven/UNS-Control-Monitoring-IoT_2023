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

request_flags_t request_read_get_http (float *value, uint8_t field) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://api.thingspeak.com/channels/" + String(THINGSPEAK_CH_ID) + "/fields/" + String(field) + "/last.json");
        int http_code = http.GET();

        if (http_code == HTTP_CODE_OK) {
            String payload = http.getString();
            int index = payload.indexOf("field" + String(field) + "\":\"") + 9;
            String value_str = payload.substring(index, index + 5);
            *value = value_str.toFloat();
            return REQUEST_HTTP_OK;
        }
    }

    return REQUEST_HTTP_ERR;
}


