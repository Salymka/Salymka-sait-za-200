#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "secrets.h"

AsyncWebServer server(8090);

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
    Serial.printf("Listing directory: %s\n", dirname);
    File root = fs.open(dirname);
    if (!root) { Serial.println("Failed to open directory"); return; }
    if (!root.isDirectory()) { Serial.println("Not a directory"); return; }
    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("DIR : ");
            Serial.println(file.name());
            if (levels) listDir(fs, file.path(), levels - 1);
        } else {
            Serial.print("FILE: "); Serial.print(file.name());
            Serial.print(" SIZE: "); Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

// Send a message to Telegram — token never leaves the ESP32
bool sendTelegram(const String& text) {
    if (WiFi.status() != WL_CONNECTED) return false;

    HTTPClient http;
    String url = "https://api.telegram.org/bot" TELEGRAM_BOT_TOKEN "/sendMessage";
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    JsonDocument doc;
    doc["chat_id"]    = TELEGRAM_CHAT_ID;
    doc["text"]       = text;
    doc["parse_mode"] = "Markdown";

    String body;
    serializeJson(doc, body);

    int code = http.POST(body);
    http.end();
    return (code == 200);
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
    if (!wm.autoConnect("Salymka-sait-za-200")) {
        Serial.println("Failed to connect");
        ESP.restart();
    }
    Serial.println("WiFi Connected!");
    Serial.println(WiFi.localIP());

    wm.stopWebPortal();
    wm.stopConfigPortal();
    delay(1000);

    // ── Static file routes ───────────────────────────────────────
    server.on("/", WebRequestMethod:: HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html", "text/html");
    });
    server.on("/style.css", WebRequestMethod:: HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/style.css", "text/css");
    });
    server.on("/app.js", WebRequestMethod:: HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/app.js", "application/javascript");
    });
    server.serveStatic("/img", LittleFS, "/img");

    // ── API: status ──────────────────────────────────────────────
    server.on("/api/status", WebRequestMethod:: HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", "{\"status\":\"online\"}");
    });

    // ── API: contact form → Telegram ─────────────────────────────
    // Receives JSON: { name, email, contact, message }
    // Token stays here in firmware — never sent to the browser
    AsyncCallbackJsonWebHandler* contactHandler =
        new AsyncCallbackJsonWebHandler("/api/contact",
            [](AsyncWebServerRequest *request, JsonVariant &json) {

                JsonObject body = json.as<JsonObject>();
                String name    = body["name"]    | "";
                String email   = body["email"]   | "";
                String contact = body["contact"] | "";
                String message = body["message"] | "";
                

                if (name.isEmpty() || email.isEmpty() || message.isEmpty()) {
                    request->send(400, "application/json",
                        "{\"ok\":false,\"error\":\"Missing fields\"}");
                    return;
                }

                String text =
                    "*New message — Handwoven*\n\n"
                    "*Name:* "  + name  + "\n" +
                    "*Email:* " + email + "\n" +
                    (contact.isEmpty() ? "" : "*Phone / Telegram:* " + contact + "\n") +
                    "\n*Message:*\n" + message;

                if (sendTelegram(text)) {
                    request->send(200, "application/json", "{\"ok\":true}");
                } else {
                    request->send(500, "application/json",
                        "{\"ok\":false,\"error\":\"Telegram send failed\"}");
                }
            });

    server.addHandler(contactHandler);
    server.begin();

    Serial.println("Server started");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println(":8090");
}

void loop() {}
