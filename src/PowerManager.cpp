#include "PowerManager.h"
#include "DeckConfig.h"
#include <M5Unified.h>
#include <time.h>

PowerManager powerManager;

void PowerManager::begin() {}

void PowerManager::loop(bool idle) {
  struct tm t;
  night = getLocalTime(&t, 2) && deckConfig.nightEnabled &&
               (deckConfig.nightStart < deckConfig.nightEnd
                 ? (t.tm_hour >= deckConfig.nightStart && t.tm_hour < deckConfig.nightEnd)
                 : (t.tm_hour >= deckConfig.nightStart || t.tm_hour < deckConfig.nightEnd));
  int target = (night || idle) ? deckConfig.idleBrightness : deckConfig.activeBrightness;
  if (target != applied) {  // also picks up brightness edits from the Web Editor
    M5.Display.setBrightness(target);
    applied = target;
  }
}
