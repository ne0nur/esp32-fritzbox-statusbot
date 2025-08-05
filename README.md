# 📶 ESP32 Fritzbox Presence Check + Telegram Bot

Dieses Projekt ermöglicht es dir, mit einem ESP32 alle verbundenen Geräte in deinem Heimnetzwerk (Fritz!Box) per Telegram-Bot zu überwachen und einfache Befehle wie `/status` und `/reboot` per Chat auszuführen.

### 🔧 Funktionen
- 📡 Abfrage aller verbundenen Geräte im Fritz!Box-Netzwerk
- 📬 Telegram-Nachrichten empfangen und antworten
- ✅ `/status` → Liste aller Geräte anzeigen
- 🔁 `/reboot` → ESP32 neustarten

---

## 🚀 Was du brauchst

- 📱 Telegram-Account + Bot (erstelle mit [@BotFather](https://t.me/botfather))
- 🔌 ESP32 Board
- 🌐 Zugang zur Fritz!Box (Benutzername + Passwort + lokale IP)
- 🔐 Arduino IDE + Bibliotheken:
  - `WiFi.h`
  - `WiFiClientSecure.h`
  - `HTTPClient.h`
  - `ArduinoJson.h`
  - `MD5Builder.h`

---

## ⚙️ Einrichtung

1. 🔌 ESP32 mit WLAN verbinden
2. ✍️ `ssid`, `password`, `fritz_ip`, `fritz_user`, `fritz_pass`, `BOT_TOKEN`, `CHAT_IDS[]` anpassen
3. 📤 Hochladen auf ESP32
4. 📲 Telegram öffnen und deinen Bot verwenden

---

## 📷 Beispiel

