#include "PowerManager.h"
#include "DeckConfig.h"
#include <M5Unified.h>
#include <time.h>

PowerManager powerManager;

void PowerManager::begin() {}

void PowerManager::loop(bool idle) {
  struct tm t;
  bool night = getLocalTime(&t, 2) && deckConfig.nightEnabled &&
               (deckConfig.nightStart < deckConfig.nightEnd
                 ? (t.tm_hour >= deckConfig.nightStart && t.tm_hour < deckConfig.nightEnd)
                 : (t.tm_hour >= deckConfig.nightStart || t.tm_hour < deckConfig.nightEnd));
  bool shouldDim = night || idle;
  if (shouldDim != dimmed) {
    M5.Display.setBrightness(shouldDim ? deckConfig.idleBrightness : deckConfig.activeBrightness);
    dimmed = shouldDim;
  }
}
