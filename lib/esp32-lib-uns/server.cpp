/*
 * server.cpp
 * Author : Muhammad Shofuwan Anwar
 * Created : 2023-12-20 00:21:00
 */

#include "server.h"

WebServer server(80);

void server_init (void) {
    server.on("/", server_handle_home);
    server.on("/configure", HTTP_GET, server_handle_configure);
    server.on("/data", server_handle_ajax);
    server.on("/ping", server_handle_ping);
    server.onNotFound(server_handle_not_found);

    server.begin();
}

void server_handle_client (void) { // handle client request
    server.handleClient();
}

void server_handle_home (void) { // show home page
   String web_html = String(R"(
    <!DOCTYPE html>
    <html>
    <head>
    <title>UNS Matching Funds Device</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
    html {
    background: linear-gradient(to bottom, #00a8ff, #192a56);
    color: #ecf0f1;
    font-family: Arial, Helvetica, sans-serif;
    height: 100%;
    }
    #home-title {
    background-color: #34495e;
    color: #ecf0f1;
    padding: 10px;
    border-radius: 5px;
    border-bottom: 10px solid #2c3e50;
    text-align: center;
    font-size: 30px;
    margin-top: 20px;
    font-weight: bold;
    }
    .container {
    display: flex;
    flex-direction: row;
    justify-content: space-around;
    align-items: center;
    flex-wrap: wrap;
    margin-top: 5px;
    }
    .cargo {
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    width: 300px;
    height: 200px;
    background-color: #34495e;
    color: #ecf0f1;
    border-radius: 15px;
    margin: 10px;
    padding: 5px;
    border: 10px solid #2c3e50;
    font-size: 25px;
    }
    #properties {
    display: flex;
    flex-direction: column;
    justify-content: space-around;
    align-items: center;
    flex-wrap: wrap;
    background-color: #34495e;
    color: #ecf0f1;
    border-radius: 5px;
    margin: 10px;
    padding: 5px;
    border-bottom: 10px solid #2c3e50;
    font-size: 25px;
    }
    #properties p {
    background-color: #415972;
    padding: 10px 30px;
    border-radius: 30px;
    margin: 8px;
    }
    #configure {
    background-color: #3498db;
    color: #ecf0f1;
    padding: 10px;
    border-radius: 5px;
    border-bottom: 10px solid #2980b9;
    text-align: center;
    font-size: 20px;
    margin-top: 30px;
    margin-bottom: 10px;
    font-weight: bold;
    text-decoration: none;
    }
    #configure:hover {
    transform: translate(2px, 2px);
    background-color: #2980b9;
    border-bottom: 10px solid #3498db;
    }
    </style>
    <script>
    function updateData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200){
    var data = JSON.parse(this.responseText);
    if (data.temperature != "" || data.temperature != "0.0") {
        document.getElementById("temp-value").innerHTML = data.temperature;
    }
    if (data.humidity != "" || data.humidity != "0.0") {
        document.getElementById("hum-value").innerHTML = data.humidity;
    }
    if (data.ph != "" || data.ph != "0.0") {
        document.getElementById("ph-value").innerHTML = data.ph;
    }
    if (data.soil != "" || data.soil != "0.0") {
        document.getElementById("soil-value").innerHTML = data.soil;
    }
    if (data.als != "" || data.als != "0.0") {
        document.getElementById("als-value").innerHTML = data.als;
    }
    
    document.getElementById("dev-id").innerHTML = data.device_id;
    document.getElementById("dev-time").innerHTML = data.device_time;
    }
    };
    xhttp.open("GET", "data", true);
    xhttp.send();
    }
    setInterval(updateData, 1000);
    </script>
    </head>
    <body>
    <div id="home-title">UNS Web Monitor</div>
    <div class="container">
    <div class="cargo">
    <p>Temperature</p>
    <p><span id="temp-value">0.0</span>&#8451;</p>
    </div>
    <div class="cargo">
    <p>Humidity</p>
    <p><span id="hum-value">0.0</span>%</p>
    </div>
    <div class="cargo">
    <p>Soil pH</p>
    <p><span id="ph-value">0.0</span></p>
    </div>
    <div class="cargo">
    <p>Soil Moisture</p>
    <p><span id="soil-value">0.0</span>%</p>
    </div>
    <div class="cargo">
    <p>Ambient Light</p>
    <p><span id="als-value">0.0</span> lux</p>
    </div>
    </div>
    <div id="properties">
    <p>Device ID : <span id="dev-id">0</span></p>
    <p>Device Up Time : <span id="dev-time">0</span></p>
    <a href="/configure" id="configure">Configure &#9881;</a>
    <a href="/ping" id="configure">Ping &#128515;</a>
    </div>
    </body>
    </html>
    )");

    server.send(200, "text/html", web_html);
}

void server_handle_configure (void) { // configure the device
    String web_html = "<!DOCTYPE html>\n"
    "<html>\n"
    "<head>\n"
    "<meta charset=\"utf-8\">\n"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
    "<title>Configure Page</title>\n"
    "<style>\n"
    "html {\n"
    "background: linear-gradient(to bottom, #00a8ff, #192a56);\n"
    "color: #ecf0f1;\n"
    "font-family: Arial, Helvetica, sans-serif;\n"
    "height: 100%;\n"
    "}\n"
    "h1 {\n"
    "background-color: #34495e;\n"
    "color: #ecf0f1;\n"
    "padding: 10px;\n"
    "border-radius: 5px;\n"
    "border-bottom: 10px solid #2c3e50;\n"
    "text-align: center;\n"
    "font-size: 30px;\n"
    "margin-top: 20px;\n"
    "font-weight: bold;\n"
    "}\n"
    "form {\n"
    "display: flex;\n"
    "flex-direction: column;\n"
    "justify-content: space-around;\n"
    "align-items: center;\n"
    "flex-wrap: wrap;\n"
    "background-color: #34495e;\n"
    "color: #ecf0f1;\n"
    "border-radius: 5px;\n"
    "margin: 10px;\n"
    "padding: 5px;\n"
    "border-bottom: 10px solid #2c3e50;\n"
    "}\n"
    "label {\n"
    "font-size: 20px;\n"
    "}\n"
    "input {\n"
    "font-size: 20px;\n"
    "padding: 5px;\n"
    "border-radius: 5px;\n"
    "border: 2px solid #2c3e50;\n"
    "}\n"
    "input[type=submit] {\n"
    "background-color: #3498db;\n"
    "color: #ecf0f1;\n"
    "padding: 10px;\n"
    "border-radius: 5px;\n"
    "border-bottom: 10px solid #2980b9;\n"
    "text-align: center;\n"
    "font-size: 20px;\n"
    "margin-top: 30px;\n"
    "margin-bottom: 10px;\n"
    "font-weight: bold;\n"
    "}\n"
    "input[type=submit]:hover {\n"
    "transform: translate(2px, 2px);\n"
    "background-color: #2980b9;\n"
    "border-bottom: 10px solid #3498db;\n"
    "}\n"
    "a {\n"
    "background-color: #3498db;\n"
    "color: #ecf0f1;\n"
    "padding: 10px;\n"
    "border-radius: 5px;\n"
    "border-bottom: 10px solid #2980b9;\n"
    "text-align: center;\n"
    "font-size: 20px;\n"
    "margin-top: 30px;\n"
    "margin-bottom: 10px;\n"
    "font-weight: bold;\n"
    "text-decoration: none;\n"
    "}\n"
    "a:hover {\n"
    "transform: translate(2px, 2px);\n"
    "background-color: #2980b9;\n"
    "border-bottom: 10px solid #3498db;\n"
    "}\n"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "<h1>Configure Page</h1>\n"
    "<form>\n"
    "<label for=\"ssid\">SSID:</label>\n"
    "<input type=\"text\" id=\"ssid\" name=\"ssid\" required><br><br>\n"
    "<label for=\"password\">Password:</label>\n"
    "<input type=\"password\" id=\"password\" name=\"password\" minlength=\"8\" required><br><br>\n"
    "<input type=\"submit\" value=\"Save\">\n"
#ifdef DEVICE_CONTROLLER_OUTSIDE
    "</form>\n"
    "<form>\n"
    "<label for=\"rname\">Relay:</label>\n"
    "<select id=\"rname\" name=\"rname\">\n"
    "<option value=\"R0\">RELAY0</option>\n"
    "<option value=\"R1\">RELAY1</option>\n"
    "</select><br>\n"
    "<label for=\"rvalue\">Value:</label>\n"
    "<select id=\"rvalue\" name=\"rvalue\">\n"
    "<option value=\"ON\">Relay ON</option>\n"
    "<option value=\"OFF\">Relay OFF</option>\n"
    "</select><br><br>\n"
    "<input type=\"submit\" value=\"Send\">\n"
#endif // DEVICE_CONTROLLER_OUTSIDE
    "<a href=\"/\">Back to Home</a>\n"
    "</form>\n"
    "</body>\n"
    "</html>\n";

    if (server.hasArg("ssid") && server.hasArg("password")) {
        server_config_t config = {
            .ssid = server.arg("ssid"),
            .password = server.arg("password")
        };

        server_create_config(&config);

        Serial.println("[WEB:FORM] SSID : " + server.arg("ssid"));
        Serial.println("[WEB:FORM] Password : " + server.arg("password"));

        server.sendHeader("Location", String("/"), true);
        server.send(302, "text/plain", "");

        ESP.restart();
    } 
#ifdef DEVICE_CONTROLLER_OUTSIDE
    else if (server.hasArg("rvalue")) {
        if (server.arg("rvalue") == "ON") {
            if (server.arg("rname") == "R0") {
                actuators_set(ACTUATORS_RELAY0, CONTROLLER_ENABLE_RELAY);
            } else if (server.arg("rname") == "R1") {
                actuators_set(ACTUATORS_RELAY1, CONTROLLER_ENABLE_RELAY);
            }
        } else if (server.arg("rvalue") == "OFF") {
            if (server.arg("rname") == "R0") {
                actuators_set(ACTUATORS_RELAY0, CONTROLLER_DISABLE_RELAY);
            } else if (server.arg("rname") == "R1") {
                actuators_set(ACTUATORS_RELAY1, CONTROLLER_DISABLE_RELAY);
            }
        }

        Serial.println("[WEB:FORM] Relay (" + server.arg("rname") + ") : " + server.arg("rvalue"));
        server.sendHeader("Location", String("/"), true);
        server.send(302, "text/plain", "");
    }
#endif // DEVICE_CONTROLLER_OUTSIDE
    else {
        server.send(200, "text/html", web_html);
    }
}

void server_handle_not_found (void) { // show 404 page
    String web_html = String(R"(
    <!DOCTYPE html>
    <html>
    <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>404 Page</title>
    <style>
    html {
    background: linear-gradient(to bottom, #00a8ff, #192a56);
    color: #ecf0f1;
    font-family: Arial, Helvetica, sans-serif;
    height: 100%;
    }
    </style>
    </head>
    <body>
    <p style="font-size: 72px;font-weight: bold;text-align: center;">(404) There's nothing here bro</p>
    </body>
    </html>
    )");

    server.send(404, "text/html", web_html);
}

void server_handle_ping (void) {
    String web_html = String(R"(
    <!DOCTYPE html>
    <html>
    <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Ping Device</title>
    <style>
    html {
    background: linear-gradient(to bottom, #00a8ff, #192a56);
    color: #ecf0f1;
    font-family: Arial, Helvetica, sans-serif;
    height: 100%;
    }
    </style>
    <script>
    alert("PONG!");
    window.location.href = "/";
    </script>
    </head>
    <body>
    </body>
    </html>
    )");

    server.send(200, "text/html", web_html);

    Serial.println("[INFO] Pinged by client.");
    const color_t color_cycle[] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE};
    uint8_t color_index = 0;

    // color cycle
    for (uint8_t x=0;x<3;x++) {
        for (uint8_t y=0;y<15;y++) {
            status_rgb_set(color_cycle[color_index]);
            vTaskDelay(50 / portTICK_PERIOD_MS);
            status_rgb_set(COLOR_NONE);
            vTaskDelay(25 / portTICK_PERIOD_MS);
        }
        color_index++;
    }
}

void server_handle_ajax (void) {
    sensors_data_t sensors_data;

    String mac_str;
    uint8_t mac[6];
    WiFi.macAddress(mac);
    for (uint8_t x=0;x<6;x++) {
        mac_str += String(mac[x], HEX);
    }

    time_keeping_t dev_time = time_keeping_get();
    time_keeping_t elapsed_mins = {
        .hour = ELAPSED_HOURS,
        .minute = ELAPSED_MINUTES,
        .second = ELAPSED_SECONDS
    };

    time_keeping_t elapsed_before = {
        .hour = ELAPSED_HOURS,
        .minute = ELAPSED_MINUTES-1,
        .second = 59
    };

    time_keeping_t elapsed_after = {
        .hour = ELAPSED_HOURS,
        .minute = ELAPSED_MINUTES,
        .second = ELAPSED_SECONDS+1
    };

    if ( (time_keeping_multiple_mins(dev_time, elapsed_mins) && (dev_time.second == elapsed_mins.second))
        || (time_keeping_multiple_mins(dev_time, elapsed_before) && (dev_time.second == elapsed_before.second))
        || (time_keeping_multiple_mins(dev_time, elapsed_after) && (dev_time.second == elapsed_after.second)) ) {
        sensors_data = (sensors_data_t) {
            .temperature = 0.0,
            .humidity = 0.0,
            .ambient_light = 0.0,
            .white_light = 0.0,
            .soil_moisture = 0.0,
            .soil_ph = 0.0
        };
    } else {
        sensors_data = sensors_sample();
    }
    
    String json = "{";
    json += "\"temperature\":";
    json += sensors_data.temperature;
    json += ",";
    json += "\"humidity\":";
    json += sensors_data.humidity;
    json += ",";
    json += "\"ph\":";
    json += sensors_data.soil_ph;
    json += ",";
    json += "\"soil\":";
    json += sensors_data.soil_moisture;
    json += ",";
    json += "\"als\":";
    json += sensors_data.ambient_light;
    json += ",";
    json += "\"device_id\":";
    json += "\"";
    json += mac_str;
    json += "\"";
    json += ",";
    json += "\"device_time\":"; 
    json += "\"";
    json += String(dev_time.hour);
    json += ":";
    json += String(dev_time.minute);
    json += ":";
    json += String(dev_time.second);
    json += "\"";
    json += "}";

    server.send(200, "application/json", json);
}

server_flags_t server_create_config (server_config_t *config) {
    // create connections.data
    File file = SPIFFS.open("/connections.data", FILE_WRITE);
    if (!file) {
        return SERVER_SPIFFS_ERR;
    }

    file.println(config->ssid);
    file.println(config->password);
    file.close();

    return SERVER_SPIFFS_OK;
}


