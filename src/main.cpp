#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFiManager.h>

AsyncWebServer server(8090);

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){

    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);

    if(!root){
        Serial.println("Failed to open directory");
        return;
    }

    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();

    while(file){

        if(file.isDirectory()){

            Serial.print("DIR : ");
            Serial.println(file.name());

            if(levels){
                listDir(fs, file.path(), levels -1);
            }

        } else {

            Serial.print("FILE: ");
            Serial.print(file.name());

            Serial.print(" SIZE: ");
            Serial.println(file.size());
        }

        file = root.openNextFile();
    }
}

void setup() {

    Serial.begin(115200);

    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    Serial.println("LittleFS mounted");

    listDir(LittleFS, "/", 5);

    WiFiManager wm;

    bool res = wm.autoConnect("ESP32-Handwoven");

    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    }

    Serial.println("");
    Serial.println("WiFi Connected!");
    Serial.println(WiFi.localIP());

    wm.stopWebPortal();
    wm.stopConfigPortal();

    delay(1000);

    server.on("/", WebRequestMethod:: HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", "text/html");
    });

    server.on("/style.css", WebRequestMethod::HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/style.css", "text/css");
    });

    server.on("/app.js", WebRequestMethod::HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/app.js", "application/javascript");
    });

    server.serveStatic("/img", LittleFS, "/img");

    server.on("/api/status", WebRequestMethod::HTTP_GET, [](AsyncWebServerRequest *request){

        request->send(
            200,
            "application/json",
            "{\"status\":\"online\"}"
        );
    });

    server.begin();

    Serial.println("Server started");
    Serial.println("Open:");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println(":8090");
}

void loop() {
}