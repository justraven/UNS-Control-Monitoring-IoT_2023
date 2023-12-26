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
    document.getElementById("temp-value").innerHTML = data.temperature;
    document.getElementById("hum-value").innerHTML = data.humidity;
    document.getElementById("ph-value").innerHTML = data.ph;
    document.getElementById("soil-value").innerHTML = data.soil;
    document.getElementById("als-value").innerHTML = data.als;
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
    </div>
    </body>
    </html>
    )");

    server.send(200, "text/html", web_html);
}

void server_handle_configure (void) { // configure the device
    String web_html = R"(
    <!DOCTYPE html>
    <html>
    <head>
    <meta charset="utf-8">
    <title>Configure Page</title>
    <style>
    html {
    background: linear-gradient(to bottom, #00a8ff, #192a56);
    color: #ecf0f1;
    font-family: Arial, Helvetica, sans-serif;
    height: 100%;
    }
    h1 {
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
    form {
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
    }
    label {
    font-size: 20px;
    }
    input {
    font-size: 20px;
    padding: 5px;
    border-radius: 5px;
    border: 2px solid #2c3e50;
    }
    input[type=submit] {
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
    }
    input[type=submit]:hover {
    transform: translate(2px, 2px);
    background-color: #2980b9;
    border-bottom: 10px solid #3498db;
    }
    a {
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
    a:hover {
    transform: translate(2px, 2px);
    background-color: #2980b9;
    border-bottom: 10px solid #3498db;
    }
    </style>
    </head>
    <body>
    <h1>Configure Page</h1>
    <form>
    <label for="ssid">SSID:</label>
    <input type="text" id="ssid" name="ssid" required><br><br>
    <label for="password">Password:</label>
    <input type="password" id="password" name="password" minlength="8" required><br><br>
    <input type="submit" value="Save">
    <a href="/">Back to Home</a>
    </form>
    </body>
    </html>
    )";

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

    server.send(200, "text/html", web_html);
}

void server_handle_not_found (void) { // show 404 page
    String web_html = String(R"(
    <!DOCTYPE html>
    <html>
    <p style="font-size: 72px;font-weight: bold;text-align: center;">(404) There's nothing here bro</p>
    </html>
    )");

    server.send(404, "text/html", web_html);
}

void server_handle_ajax (void) {
    sensors_data_t sensors_data = sensors_sample();

    String mac_str;
    uint8_t mac[6];
    WiFi.macAddress(mac);
    for (uint8_t x=0;x<6;x++) {
        mac_str += String(mac[x], HEX);
    }

    time_keeping_t dev_time = time_keeping_get();
    
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


