# 📶 ESP32 Fritzbox Presence Check + Telegram Bot

Dieses Projekt ermöglicht es dir, mit einem ESP32 alle verbundenen Geräte in deinem Heimnetzwerk (Fritz!Box) per Telegram-Bot zu überwachen und einfache Befehle wie `/status` und `/reboot` per Chat auszuführen.

### 🔧 Funktionen
- 📶 WLAN-Verbindung: Verbindet sich bei Start automatisch mit dem WLAN.
- 🤖 Telegram-Bot-Integration: Reagiert auf Befehle von autorisierten Nutzern via Telegram.
- 🔐 Fritzbox-Login: Führt ein MD5-basiertes Login durch, um eine gültige Session-ID (SID) zu erhalten.
- 📋 Geräteliste abrufen: Holt die aktuell verbundenen Geräte von der Fritzbox-Weboberfläche.
- 💬 Antwort über Telegram: Sendet die Liste der verbundenen Geräte als formatierte Nachricht an den Telegram-Chat.
- ✅ `/status` → Liste aller Geräte anzeigen
- 🔁 `/reboot` → ESP32 neustarten

### Technologien & Bibliotheken:
- `WiFi.h` & `WiFiClientSecure.h` für WLAN
- `HTTPClient.h` für Webanfragen
- `ArduinoJson.h` zum Parsen der Telegram-Antworten
- `MD5Builder.h` für die Fritzbox-Authentifizierung
- Telegram Bot API für Messaging
- Fritzbox-Login-Skript via login_sid.lua

  
---

## 🚀 Schnellstart

### 1. Voraussetzungen
- ESP32 (z. B. ESP32-S3 DevKitC)
- Arduino IDE installiert
- WLAN-Zugangsdaten
- Zugang zur Fritz!Box + Benutzername + Passwort → `in secrets.h eintragen ❗`
- Telegram-Bot & Chat-ID(s)
### 2. Telegram-Bot erstellen
1. Schreibe an [@BotFather](https://t.me/botfather)
2. Sende `/newbot`
3. Vergib einen Namen und Benutzernamen (endet auf `bot`)
4. Du bekommst einen Token – diesen im Code bei `BOT_TOKEN` → `in secrets.h eintragen ❗`
5. Starte den Bot in deinem Telegram-Account und sende `/start`

### 🆔 Telegram Chat-ID herausfinden

**Variante 1 (einfacher):**
1. Öffne [@getidsbot](https://t.me/getidsbot)
2. Sende `/start`
3. Notiere dir die angezeigte `Chat-ID` → `in secrets.h eintragen ❗`

**Variante 2 (manuell über Telegram API):**
1. Sende deinem Bot eine Nachricht
2. Öffne folgenden Link im Browser:  
   `https://api.telegram.org/bot<dein_bot_token>/getUpdates`
3. Suche nach `chat.id` im angezeigten Text

### 4. FritzBox vorbereiten
1. Auf der FritzBox einen Benutzer anlegen
2. "Zugang auch aus dem Heimnetz zulassen"
3. Benutzername und Passwort → `in secrets.h eintragen ❗`
4. IP-Adresse der FritzBox ggf. anpassen (`192.168.178.1`)

### 5. Projekt kompilieren & hochladen
1. Arduino IDE öffnen
2. Bibliotheken installieren:
   - `WiFi`
   - `WiFiClientSecure`
   - `ArduinoJson`
   - `HTTPClient`
   - `MD5Builder`
3. Board: `ESP32 Modul` auswählen
4. ❗❗`Sketch.ino` und `secrets.h` müssen im selben Ordner sein❗❗
5. Sketch hochladen

### 6. Telegram-Befehle
- `/status` → zeigt verbundene Geräte an
- `/reboot` → startet den ESP32 neu

Die `Baudrate 115200` ist Standard für die Kommunikation mit ESP32/Arduino über den seriellen Monitor.

---

## 📷 Beispiel
![screen](https://github.com/user-attachments/assets/c43e9c81-54ea-4d66-954d-6caaac7352ae)


