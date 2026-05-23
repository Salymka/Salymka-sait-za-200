# ESP32 Handwoven Website

## Features

- ESP32 Web Server
- WiFiManager auto connect portal
- LittleFS filesystem
- Mobile responsive
- Local images
- Async web server
- REST API example

## Upload Steps

### 1. Upload filesystem

pio run --target uploadfs

### 2. Upload firmware

pio run --target upload

## First Boot

ESP32 creates WiFi hotspot:

ESP32-Handwoven

Connect to it and configure your WiFi.

## Open Website

Check Serial Monitor for IP address.
