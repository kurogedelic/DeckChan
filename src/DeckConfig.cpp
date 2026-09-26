#include "DeckConfig.h"
#include <Preferences.h>
#include <ArduinoJson.h>

DeckConfig deckConfig;
static Preferences prefs;

void applyDeckPalette() {
  deckConfig.background = 0x0000;
  if (deckConfig.palette == "green") deckConfig.foreground = 0x07E0;
  else if (deckConfig.palette == "white") deckConfig.foreground = 0xFFFF;
  else if (deckConfig.palette == "ice") deckConfig.foreground = 0xBFFF;
  else deckConfig.foreground = 0xFD20;
}

String deckConfigJson(bool includeSecret) {
  JsonDocument d;
  d["deviceName"]=deckConfig.deviceName; d["palette"]=deckConfig.palette;
  d["idle"]=deckConfig.idleAfterMs/1000; d["refresh"]=deckConfig.refreshSeconds;
  d["seconds"]=deckConfig.showSeconds; d["weatherUrl"]=deckConfig.weatherUrl;
  d["calendarUrl"]=deckConfig.calendarUrl; d["homebridgeUrl"]=deckConfig.homebridgeUrl;
  if (includeSecret) d["homebridgeToken"]=deckConfig.homebridgeToken;
  d["activeBrightness"]=deckConfig.activeBrightness; d["idleBrightness"]=deckConfig.idleBrightness;
  d["nightEnabled"]=deckConfig.nightEnabled; d["nightStart"]=deckConfig.nightStart; d["nightEnd"]=deckConfig.nightEnd;
  String out; serializeJson(d,out); return out;
}

bool updateDeckConfigJson(const String& json) {
  JsonDocument d; if (deserializeJson(d,json)) return false;
  if (d["deviceName"].is<String>()) deckConfig.deviceName=d["deviceName"].as<String>();
  if (d["palette"].is<String>()) deckConfig.palette=d["palette"].as<String>();
  if (d["idle"].is<int>()) deckConfig.idleAfterMs=max(5,d["idle"].as<int>())*1000UL;
  if (d["refresh"].is<int>()) deckConfig.refreshSeconds=max(30,d["refresh"].as<int>());
  if (d["seconds"].is<bool>()) deckConfig.showSeconds=d["seconds"];
  if (d["weatherUrl"].is<String>()) deckConfig.weatherUrl=d["weatherUrl"].as<String>();
  if (d["calendarUrl"].is<String>()) deckConfig.calendarUrl=d["calendarUrl"].as<String>();
  if (d["homebridgeUrl"].is<String>()) deckConfig.homebridgeUrl=d["homebridgeUrl"].as<String>();
  if (d["homebridgeToken"].is<String>() && d["homebridgeToken"].as<String>().length()) deckConfig.homebridgeToken=d["homebridgeToken"].as<String>();
  if (d["activeBrightness"].is<int>()) deckConfig.activeBrightness=constrain(d["activeBrightness"].as<int>(),1,255);
  if (d["idleBrightness"].is<int>()) deckConfig.idleBrightness=constrain(d["idleBrightness"].as<int>(),1,255);
  if (d["nightEnabled"].is<bool>()) deckConfig.nightEnabled=d["nightEnabled"];
  if (d["nightStart"].is<int>()) deckConfig.nightStart=constrain(d["nightStart"].as<int>(),0,23);
  if (d["nightEnd"].is<int>()) deckConfig.nightEnd=constrain(d["nightEnd"].as<int>(),0,23);
  applyDeckPalette(); return true;
}

bool saveDeckConfig() {
  prefs.begin("deckchan",false);
  String json=deckConfigJson(true);
  bool ok=prefs.putString("config",json)>0; prefs.end(); return ok;
}

bool loadDeckConfig() {
  prefs.begin("deckchan",true); String json=prefs.getString("config",""); prefs.end();
  if (!json.length()) { applyDeckPalette(); return false; }
  return updateDeckConfigJson(json);
}
