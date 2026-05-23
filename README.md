# ESP32 Static Website Host

Host modern static websites directly from an ESP32 using LittleFS.

This project provides a lightweight web server with automatic WiFi configuration via WiFiManager, allowing you to quickly deploy landing pages, dashboards, product showcases, IoT control panels, and other static websites without requiring an external server.

---

## Features

- ESP32 Web Server
- WiFiManager auto-configuration portal
- LittleFS filesystem support
- HTML, CSS, JavaScript and image hosting
- Mobile responsive websites
- REST API example
- Fast startup
- No external backend required
- Easy website customization

---

## Project Structure

```text
.
├── src/
│   └── main.cpp
├── data/
│   ├── index.html
│   ├── style.css
│   ├── app.js
│   └── img/
│       ├── hero.webp
│       ├── image.webp
│       └── ...
└── platformio.ini
```

All files inside the `data` directory are uploaded to LittleFS and served by the ESP32.

---

## AI Website Generation Prompt

Use the following prompt to generate websites compatible with this project:

```text
Create a modern responsive website for ESP32 hosting.

Requirements:

1. Generate exactly 3 files:
   - index.html
   - style.css
   - app.js

2. Use:
   - Pure HTML
   - Pure CSS
   - Pure JavaScript

3. Do not use:
   - React
   - Vue
   - Angular
   - Bootstrap
   - Tailwind
   - Node.js
   - Any external dependencies

4. Design requirements:
   - Modern UI
   - Mobile-first responsive layout
   - Smooth animations
   - Professional appearance
   - Fast loading
   - Clean typography

5. Image handling(write what you use!):

   - Local images must use:
     /img/image.webp
     /img/hero.webp
     /img/product.webp

   - If no local images are available,
     use external image URLs(list).
     https://...hero.webp
     https://...image2.webp
     

6. JavaScript:
   - Vanilla JavaScript only
   - No build tools
   - No package managers

7. ESP32 compatibility:
   - Static website only
   - No server-side code
   - No dependencies
   - Must work directly from LittleFS

8. Performance:
   - Optimize for ESP32 hosting
   - Minimize HTTP requests

Generate the complete website.
```

---

## Build Requirements

### PlatformIO

Required libraries:

```ini
lib_deps =
    ESP32Async/ESPAsyncWebServer
    ESP32Async/AsyncTCP
    bblanchon/ArduinoJson
    tzapu/WiFiManager
```

---

## Upload Website Files

Upload the contents of the `data` folder to LittleFS:

```bash
pio run --target uploadfs
```

---

## Upload Firmware

Flash the firmware to ESP32:

```bash
pio run --target upload
```

---

## First Startup

On first boot the ESP32 creates a WiFi configuration access point:

```text
Salymka-sait-za-200
```

---

## Connect ESP32 to WiFi

1. Connect your phone or computer to:

```text
Salymka-sait-za-200
```

2. WiFiManager portal should open automatically.

If it does not open, navigate to:

```text
http://192.168.4.1
```

3. Select your WiFi network.

4. Enter your WiFi password.

5. Save configuration.

6. The ESP32 will restart and connect to your network.

---

## Open the Website

Open Serial Monitor:

```bash
pio device monitor
```

After successful connection you should see:

```text
WiFi Connected!
192.168.xxx.xxx

Server started
Open:
http://192.168.xxx.xxx:8090
```

Open the displayed address in your browser.

Example:

```text
http://192.168.1.105:8090
```

---

### API Example

```http
GET /api/status
```

Response:

```json
{
  "status": "online"
}
```

---

## Updating the Website

Modify files inside:

```text
data/
```

Then upload the filesystem again:

```bash
pio run --target uploadfs
```

Firmware upload is not required unless the source code changes.

---

## Use Cases

- Landing Pages
- Product Showcases
- IoT Dashboards
- Device Configuration Panels
- Smart Home Interfaces
- Portfolio Websites
- Offline Kiosks
- Interactive Demonstrations
- Local Information Displays

---

## License

MIT License

---

Built with ESP32, LittleFS, ESPAsyncWebServer and WiFiManager.