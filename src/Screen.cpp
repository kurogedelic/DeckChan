#include "Screen.h"
#include "Face.h"
#include <string.h>

static Face face;

static uint16_t dimColor(uint16_t c) {
  return ((c >> 1) & 0x7BEF);  // half intensity per RGB565 channel
}

// Draws text clipped to maxWidth px, cutting on character count so it never spills.
static void fitString(lgfx::LGFX_Sprite& c, const char* text, int x, int y, int maxWidth) {
  int charW = 6 * c.getTextSizeX();
  size_t n = maxWidth / charW;
  char buf[64];
  size_t len = strlen(text);
  if (n >= sizeof(buf)) n = sizeof(buf) - 1;
  if (len > n) len = n;
  memcpy(buf, text, len);
  buf[len] = 0;
  c.drawString(buf, x, y);
}

static void statusBar(lgfx::LGFX_Sprite& c, const ScreenModel& m) {
  c.setTextSize(1);
  c.setTextColor(dimColor(m.foreground), m.background);
  c.drawString(m.date, 12, 8);
  char right[24];
  if (m.batteryPercent >= 0)
    snprintf(right, sizeof(right), "%s  %s%d%%", m.online ? "NET" : "---", m.charging ? "+" : "", m.batteryPercent);
  else
    snprintf(right, sizeof(right), "%s", m.online ? "NET" : "---");
  c.setTextDatum(top_right);
  c.drawString(right, c.width() - 12, 8);
  c.setTextDatum(top_left);
  c.setTextColor(m.foreground, m.background);
}

static void card(lgfx::LGFX_Sprite& c, const ScreenModel& m, int x, int y, int w, int h, const char* title, const char* value) {
  c.drawRect(x, y, w, h, m.foreground);
  c.setTextSize(1);
  c.setTextColor(dimColor(m.foreground), m.background);
  c.drawString(title, x + 7, y + 7);
  c.setTextColor(m.foreground, m.background);
  c.setTextSize(2);
  fitString(c, value, x + 7, y + 24, w - 14);
}

static void drawIdle(lgfx::LGFX_Sprite& c, const ScreenModel& m) {
  c.setTextSize(6);
  c.setTextDatum(top_center);
  c.drawString(m.clock, c.width() / 2, 30);
  c.setTextDatum(top_left);
  face.draw(c, (c.width() - Face::width) / 2, 118, m.foreground, m.blink);
  c.drawFastHLine(12, 196, c.width() - 24, dimColor(m.foreground));
  c.setTextSize(1);
  c.setTextColor(dimColor(m.foreground), m.background);
  c.drawString("DECKCHAN // IDLE", 12, 212);
  c.setTextDatum(top_right);
  c.drawString(m.weather, c.width() - 12, 212);
  c.setTextDatum(top_left);
}

static void drawDashboard(lgfx::LGFX_Sprite& c, const ScreenModel& m) {
  c.setTextSize(3);
  c.drawString(m.clock, 12, 24);
  card(c, m, 12, 62, 142, 60, "WEATHER", m.weather);
  card(c, m, 166, 62, 142, 60, "NEXT", m.calendar);
  card(c, m, 12, 134, 296, 60, "HOME", m.home);
  c.setTextSize(1);
  c.setTextColor(dimColor(m.foreground), m.background);
  c.drawString("DECKCHAN // DASHBOARD", 12, 212);
}

static void drawMessage(lgfx::LGFX_Sprite& c, const ScreenModel& m) {
  c.setTextSize(3);
  c.drawString(m.clock, 12, 24);
  c.drawRect(12, 62, 296, 132, m.foreground);
  c.setTextSize(1);
  c.setTextColor(dimColor(m.foreground), m.background);
  c.drawString("MESSAGE", 19, 69);
  c.setTextColor(m.foreground, m.background);
  c.setTextSize(2);
  // Word wrap at 23 chars per line (hard break for long words), up to 5 lines.
  const size_t cols = 23;
  const char* p = m.message;
  for (int line = 0; line < 5 && *p; line++) {
    size_t len = strlen(p), n = len;
    if (len > cols) {
      n = cols;
      while (n > 0 && p[n] != ' ') n--;
      if (n == 0) n = cols;
    }
    char buf[cols + 1];
    memcpy(buf, p, n);
    buf[n] = 0;
    c.drawString(buf, 19, 88 + line * 20);
    p += n;
    while (*p == ' ') p++;
  }
}

void drawScreen(lgfx::LGFX_Sprite& c, const ScreenModel& m) {
  c.fillScreen(m.background);
  c.setTextFont(1);
  c.setTextDatum(top_left);
  c.setTextColor(m.foreground, m.background);
  statusBar(c, m);
  if (m.message && *m.message) drawMessage(c, m);
  else if (m.dashboard) drawDashboard(c, m);
  else drawIdle(c, m);
}
