# ESP32 Static Website Host

Host modern static websites directly from an ESP32 using LittleFS — no external server required. The contact form sends messages to Telegram via a bot, with the token stored securely in firmware.

---

## Features

- Serves HTML, CSS, JS and images from LittleFS
- Auto WiFi setup via WiFiManager captive portal
- Contact form → Telegram notifications (token never exposed to browser)
- Mobile responsive, no frameworks required
- No external backend or cloud dependency

---

## Project Structure

```text
.
├── src/
│   └── main.cpp              # Web server + Telegram handler
├── include/
│   ├── secrets.h             # Your tokens — never commit this
│   └── secrets.h.example     # Safe template to commit
├── data/
│   ├── index.html
│   ├── style.css
│   ├── app.js
│   └── img/
│       ├── hero.webp
│       └── ...
├── example-prompt/
│   └── prompt.md             # AI prompt to generate new sites
└── platformio.ini
```

---

## Telegram Setup

The ESP32 sends contact form submissions directly to a Telegram chat.

**1. Create a bot**

Open Telegram, find `@BotFather` and run `/newbot`. Copy the token you receive.

**2. Get your chat ID**

Send any message to your bot, then open in your browser:

```
https://api.telegram.org/bot<YOUR_TOKEN>/getUpdates
```

Find `"chat": { "id": ... }` in the response — that is your chat ID.

Or find chatId bot in telegram.

**3. Fill in `include/secrets.h`**

Copy `secrets.h.example` → `secrets.h` and fill in your values:

```cpp
#define TELEGRAM_BOT_TOKEN  "123456789:AAxxxxxx..."
#define TELEGRAM_CHAT_ID    "987654321"
```

`secrets.h` is in `.gitignore` and will never be committed.

---

## Getting Started

**1. Install dependencies**

```ini
lib_deps =
    ESP32Async/ESPAsyncWebServer
    ESP32Async/AsyncTCP
    bblanchon/ArduinoJson
    tzapu/WiFiManager
```

**2. Add your secrets**

```bash
cp include/secrets.h.example include/secrets.h
# edit include/secrets.h with your token and chat ID
```

**3. Upload the filesystem**

```bash
pio run --target uploadfs
```

**4. Flash the firmware**

```bash
pio run --target upload
```

---

## First Boot — WiFi Setup

On first boot the ESP32 creates a WiFi access point:

```
Salymka-sait-za-200
```

Connect to it with your phone or computer. A configuration portal opens automatically — select your network, enter the password, save. The ESP32 restarts and connects.

---

## Finding the IP Address

Open Serial Monitor after boot:

```bash
pio device monitor
```

You will see:

```
WiFi Connected!
192.168.1.xxx
Server started
http://192.168.1.xxx:8090
```

Open that address in your browser.

---

## Updating the Website

Edit files in `data/`, then re-upload the filesystem:

```bash
pio run --target uploadfs
```

Firmware re-flash is only needed if `main.cpp` changes.

---

## Generating a New Website with AI

A ready-to-use AI prompt is in:

```
example-prompt/prompt.md
```

Fill in your business details and paste into Claude (or any AI) to generate a matching `index.html`, `style.css` and `app.js` in one shot.

---

## Use Cases

- Landing pages & product showcases
- Portfolio websites
- IoT dashboards & device control panels
- Smart home interfaces
- Offline kiosks & local info displays

---
