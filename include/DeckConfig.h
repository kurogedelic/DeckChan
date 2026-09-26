#pragma once
#include <Arduino.h>

struct DeckConfig {
  uint16_t foreground = 0xFD20;
  uint16_t background = 0x0000;
  uint32_t idleAfterMs = 20000;
  uint32_t refreshSeconds = 300;
  bool showSeconds = false;
  String deviceName = "DeckChan";
  String palette = "amber";
  String weatherUrl = "";
  String calendarUrl = "";
  String homebridgeUrl = "";
  String homebridgeToken = "";
  uint8_t activeBrightness = 180;
  uint8_t idleBrightness = 45;
  bool nightEnabled = true;
  uint8_t nightStart = 0;
  uint8_t nightEnd = 7;
  bool motionEnabled = true;  // head nod on wake (servos via StackChan-BSP)
  bool ledsEnabled = true;    // body RGB LEDs glow while a message is shown
};

extern DeckConfig deckConfig;
bool loadDeckConfig();
bool saveDeckConfig();
void applyDeckPalette();
String deckConfigJson(bool includeSecret = false);
bool updateDeckConfigJson(const String& json);
