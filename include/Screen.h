#pragma once
#include <M5GFX.h>

// Plain snapshot of everything the screen shows. Kept free of Arduino types so
// the renderer also builds on the host (see sim/) for screenshots.
struct ScreenModel {
  const char* clock = "--:--";
  const char* date = "";
  const char* weather = "--";
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
