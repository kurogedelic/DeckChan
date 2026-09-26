// Host renderer: draws DeckChan screens with the real M5GFX + Screen.cpp and
// writes PPM files, so layouts can be reviewed without hardware.
#include <M5GFX.h>
#include <cstdio>
#include "Screen.h"

static void dump(M5Canvas& c, const char* path) {
  FILE* f = fopen(path, "wb");
  fprintf(f, "P6 %d %d 255\n", c.width(), c.height());
  for (int y = 0; y < c.height(); y++)
    for (int x = 0; x < c.width(); x++) {
      uint16_t p = c.readPixel(x, y);
      fputc(((p >> 11) & 31) * 255 / 31, f);
      fputc(((p >> 5) & 63) * 255 / 63, f);
      fputc((p & 31) * 255 / 31, f);
    }
  fclose(f);
}

int main(int argc, char** argv) {
  const char* out = argc > 1 ? argv[1] : ".";
  M5Canvas c;
  c.setColorDepth(16);
  c.createSprite(320, 240);
  ScreenModel m;
  m.clock = "09:41"; m.online = true; m.batteryPercent = 82;
  m.temperature = 21.4f; m.weatherCode = 2; m.calendar = "10:30 REVIEW"; m.home = "LIGHTS ON";
  char path[256];
  struct { const char* name; uint16_t fg; } palettes[] = {{"amber", 0xFD20}, {"green", 0x07E0}, {"ice", 0xBFFF}};
  for (auto& p : palettes) {
    m.foreground = p.fg;
    m.dashboard = false; m.message = "";
    drawScreen(c, m); snprintf(path, sizeof(path), "%s/idle-%s.ppm", out, p.name); dump(c, path);
    m.dashboard = true;
    drawScreen(c, m); snprintf(path, sizeof(path), "%s/dashboard-%s.ppm", out, p.name); dump(c, path);
  }
  m.foreground = 0xFD20; m.message = "Laundry is done.";
  drawScreen(c, m); snprintf(path, sizeof(path), "%s/message-amber.ppm", out); dump(c, path);
  return 0;
}
