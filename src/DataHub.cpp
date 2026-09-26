#include "DataHub.h"
#include "DeckConfig.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

DataHub dataHub;

String DataHub::httpGet(const String& url, const String& auth) {
  if (!url.length() || WiFi.status() != WL_CONNECTED) return "";
  HTTPClient http;
  http.setTimeout(3500);
  if (!http.begin(url)) return "";
  if (auth.length()) http.addHeader("Authorization", auth);
  int code = http.GET();
  String body = code >= 200 && code < 300 ? http.getString() : "";
  http.end();
  return body;
}

void DataHub::begin() { refresh(); }

void DataHub::loop() {
  if (millis() - lastRefresh > deckConfig.refreshSeconds * 1000UL) refresh();
  // One source per pass keeps each loop() stall to a single HTTP timeout.
  switch (pending) {
    case 1: fetchWeather(); pending++; break;
    case 2: fetchCalendar(); pending++; break;
    case 3: fetchHomebridge(); pending = 0; break;
  }
  if (state.messageUntil && (int32_t)(millis() - state.messageUntil) >= 0) {
    state.message = "";
    state.messageUntil = 0;
  }
}

void DataHub::refresh() {
  lastRefresh = millis();
  pending = 1;
}

void DataHub::notify(const String& text, uint32_t seconds) {
  state.message = text.substring(0, 80);
  state.messageUntil = (millis() + seconds * 1000UL) | 1;  // never 0 (0 = no message)
}

void DataHub::fetchWeather() {
  String body = httpGet(deckConfig.weatherUrl);
  if (!body.length()) return;
  JsonDocument doc;
  if (deserializeJson(doc, body)) return;
  // Open-Meteo compatible: current.temperature_2m (+ current.weather_code for the icon)
  if (!doc["current"]["temperature_2m"].isNull()) {
    state.temperature = doc["current"]["temperature_2m"].as<float>();
    state.weatherCode = doc["current"]["weather_code"] | -1;
    state.weather = String(state.temperature, 1) + " C";
  } else if (!doc["temperature"].isNull()) {
    state.weather = doc["temperature"].as<String>();
  }
}

void DataHub::fetchCalendar() {
  String body = httpGet(deckConfig.calendarUrl);
  if (!body.length()) return;
  JsonDocument doc;
  if (!deserializeJson(doc, body)) {
    if (!doc["next"].isNull()) state.calendar = doc["next"].as<String>().substring(0, 24);
    return;
  }
  // Also accepts a tiny plain-text endpoint for local bridges.
  state.calendar = body.substring(0, 24);
}

void DataHub::fetchHomebridge() {
  String body = httpGet(deckConfig.homebridgeUrl, deckConfig.homebridgeToken.length() ? "Bearer " + deckConfig.homebridgeToken : "");
  if (!body.length()) return;
  JsonDocument doc;
  if (!deserializeJson(doc, body) && !doc["status"].isNull()) {
    state.home = doc["status"].as<String>().substring(0, 32);
  } else {
    state.home = body.substring(0, 32);
  }
}
