#pragma once
#include <Arduino.h>

struct DeckConfig {
  uint16_t foreground = 0xFD20; // amber RGB565
  uint16_t background = 0x0000;
  uint32_t idleAfterMs = 20000;
  bool showSeconds = false;
  String deviceName = "DeckChan";
};

extern DeckConfig deckConfig;
