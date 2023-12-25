/*
 * connections.cpp
 * Author : Muhammad Shofuwan Anwar
 * Created : 2023-12-20 00:21:00
 */

#include "connections.h"

connections_flags_t connections_init (void) {
    if (!SPIFFS.begin(true)) {
        return CONNECTIONS_SPIFFS_ERR;
    }

    return CONNECTIONS_SPIFFS_OK;
}

connections_flags_t connections_read_config (connections_hwdata_t *hwdata) {
    // read data from connections.data
    File file = SPIFFS.open("/connections.data", FILE_READ);
    if (!file) {
        return CONNECTIONS_SPIFFS_ERR_CONFIG;
    }

    String ssid = file.readStringUntil('\n');
    String password = file.readStringUntil('\n');

    if (ssid.length() < 3 || password.length() < 3) {
        file.close();
        return CONNECTIONS_SPIFFS_ERR_CONFIG;
    }

    hwdata->ssid = ssid;
    hwdata->password = password;

    file.close();
    return CONNECTIONS_SPIFFS_OK;
}

connections_flags_t connections_start_ap (connections_hwdata_t *hwdata) {
    String ssid = CONNECTIONS_AP_SSID;
    String tmp_ipaddr = "";
    String tmp_macaddr = "";

    uint8_t mac[6];
    WiFi.macAddress(mac);
    for (uint8_t x=0;x<6;x++) {
        ssid += String(mac[x], HEX);
        tmp_macaddr += String(mac[x], HEX);
    }

    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid.c_str(), CONNECTIONS_AP_PASS);
    WiFi.hostname(ssid.c_str());

    tmp_ipaddr = WiFi.softAPIP().toString();

    hwdata->ipaddr = tmp_ipaddr;
    hwdata->macaddr = tmp_macaddr;

    return CONNECTIONS_SUCCESS;
}



