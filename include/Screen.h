#pragma once
#include <M5GFX.h>
#include <math.h>

// Plain snapshot of everything the screen shows. Kept free of Arduino types so
// the renderer also builds on the host (see sim/) for screenshots.
struct ScreenModel {
  const char* clock = "--:--";
  float temperature = NAN;   // shown with an icon when known
  int weatherCode = -1;      // WMO code (Open-Meteo), picks the icon
  const char* weather = "--"; // fallback text for non-Open-Meteo sources
  const char* calendar = "NO DATA";
  const char* home = "NO DATA";
  const char* message = "";
  bool dashboard = false;
  bool blink = false;
  bool online = false;
  int batteryPercent = -1;  // -1 = unknown
  bool charging = false;
  uint16_t foreground = 0xFD20;
  uint16_t background = 0x0000;
};

void drawScreen(lgfx::LGFX_Sprite& canvas, const ScreenModel& m);
