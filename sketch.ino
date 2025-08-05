#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <MD5Builder.h>

// Eigene Zugangsdaten – ausgelagert in secrets.h
#include "secrets.h"

// Telegram API
const String BOT_TOKEN = BOT_TOKEN_SECRET;
const String TELEGRAM_API = "https://api.telegram.org/bot" + BOT_TOKEN;

// Zeitsteuerung
unsigned long lastCheck = 0;
const unsigned long checkInterval = 5000; // alle 5 Sekunden prüfen

// Session ID für Fritzbox
String SID = "";

// Funktionen für Telegram-Bot
void sendTelegramMessage(String message, String chat_id) {
  HTTPClient http;
  String url = TELEGRAM_API + "/sendMessage?chat_id=" + chat_id + "&text=" + urlencode(message);
  http.begin(url);
  http.GET();
  http.end();
}

// Funktion zur URL-Codierung (Telegram erwartet UTF-8)
String urlencode(String str) {
  String encoded = "";
  char c;
  char code0, code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) code1 = (c & 0xf) - 10 + 'A';
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) code0 = c - 10 + 'A';
      encoded += '%';
      encoded += code0;
      encoded += code1;
    }
  }
  return encoded;
}

// Authentifizierung an Fritzbox – MD5 Login
bool fritzLogin() {
  HTTPClient http;
  http.begin("http://" + FRITZBOX_IP + "/login_sid.lua");
  int httpCode = http.GET();
  if (httpCode == 200) {
    String payload = http.getString();
    int idxStart = payload.indexOf("<Challenge>") + 11;
    int idxEnd = payload.indexOf("</Challenge>");
    String challenge = payload.substring(idxStart, idxEnd);

    MD5Builder md5;
    md5.begin();
    md5.add(challenge + "-" + FRITZBOX_PASSWORD);
    md5.calculate();
    String response = challenge + "-" + md5.toString();

    // Login mit Passwort-Hash
    String url = "http://" + FRITZBOX_IP + "/login_sid.lua?username=" + FRITZBOX_USER + "&response=" + response;
    http.begin(url);
    http.GET();
    String sidXml = http.getString();

    idxStart = sidXml.indexOf("<SID>") + 5;
    idxEnd = sidXml.indexOf("</SID>");
    SID = sidXml.substring(idxStart, idxEnd);

    http.end();
    return SID != "0000000000000000";
  }
  http.end();
  return false;
}

// Abruf der verbundenen Geräte von Fritzbox
String getConnectedDevices() {
  HTTPClient http;
  String deviceList = "";
  String url = "http://" + FRITZBOX_IP + "/net/network_user_devices.lua?sid=" + SID;
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode == 200) {
    String html = http.getString();

    // Beispiel: einfache Parsing-Strategie (nicht perfekt)
    int idx = 0;
    while ((idx = html.indexOf("Device:", idx)) != -1) {
      int nameStart = html.indexOf("Name: ", idx) + 6;
      int nameEnd = html.indexOf("<", nameStart);
      String name = html.substring(nameStart, nameEnd);

      int ipStart = html.indexOf("IP: ", nameEnd) + 4;
      int ipEnd = html.indexOf("<", ipStart);
      String ip = html.substring(ipStart, ipEnd);

      int macStart = html.indexOf("MAC: ", ipEnd) + 5;
      int macEnd = html.indexOf("<", macStart);
      String mac = html.substring(macStart, macEnd);

      deviceList += "📱 " + name + "\nMAC: `" + mac + "`\nIP: `" + ip + "`\n\n";
      idx = macEnd;
    }
  } else {
    deviceList = "❌ Fehler beim Abruf!";
  }
  http.end();
  return deviceList;
}

// Telegram-Befehle abfragen
void checkTelegramMessages() {
  HTTPClient http;
  String url = TELEGRAM_API + "/getUpdates";
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(4096);
    deserializeJson(doc, payload);

    for (JsonObject update : doc["result"].as<JsonArray>()) {
      String message = update["message"]["text"];
      String chat_id = String(update["message"]["chat"]["id"]);

      if (!isChatAllowed(chat_id)) return;

      if (message == "/status") {
        if (fritzLogin()) {
          String devices = getConnectedDevices();
          sendTelegramMessage(devices, chat_id);
        } else {
          sendTelegramMessage("⚠️ Login zur Fritzbox fehlgeschlagen", chat_id);
        }
      }

      if (message == "/reboot") {
        sendTelegramMessage("🔄 ESP32 wird neugestartet...", chat_id);
        delay(1000);
        ESP.restart();
      }
    }
  }
  http.end();
}

// Nur bestimmte Chat-IDs zulassen
bool isChatAllowed(String chat_id) {
  for (int i = 0; i < sizeof(CHAT_IDS) / sizeof(CHAT_IDS[0]); i++) {
    if (chat_id == CHAT_IDS[i]) return true;
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WLAN verbunden!");
}

void loop() {
  if (millis() - lastCheck > checkInterval) {
    checkTelegramMessages();
    lastCheck = millis();
  }
}

