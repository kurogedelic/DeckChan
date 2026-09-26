#include "Screen.h"
#include "Bitmaps.h"
#include "Face.h"
#include <stdio.h>
#include <string.h>

// The CoreS3 panel is 2.0" at 320x240, so the smallest text used is 2x (16 px).
static Face face;

static uint16_t dimColor(uint16_t c) { return (c >> 1) & 0x7BEF; }

static const Bitmap& weatherIcon(int code) {
  if (code < 0 || code <= 1) return icons::sun;
  if (code >= 95) return icons::thunder;
  if ((code >= 71 && code <= 77) || code == 85 || code == 86) return icons::snow;
  if (code >= 51) return icons::rain;
  return icons::cloud;
}

// Draws text cut to maxWidth px so it never runs off the panel.
static void fitString(lgfx::LGFX_Sprite& c, const char* text, int x, int y, int maxWidth) {
  char buf[48];
  size_t n = maxWidth / (6 * c.getTextSizeX()), len = strlen(text);
  if (n >= sizeof(buf)) n = sizeof(buf) - 1;
  if (len > n) len = n;
  memcpy(buf, text, len);
  buf[len] = 0;
  c.drawString(buf, x, y);
}

static void batteryIcon(lgfx::LGFX_Sprite& c, int x, int y, int percent, bool charging, uint16_t fg) {
  c.drawRect(x, y, 26, 14, fg);
  c.fillRect(x + 26, y + 4, 3, 6, fg);
  int w = percent < 0 ? 0 : (22 * percent + 50) / 100;
  if (charging) w = 22;
  c.fillRect(x + 2, y + 2, w, 10, charging ? dimColor(fg) : fg);
}

// Clock on the left, Wi-Fi + battery on the right. Shared by dashboard and message.
static void header(lgfx::LGFX_Sprite& c, const ScreenModel& m) {
  c.setTextSize(4);
  c.drawString(m.clock, 12, 10);
  int x = c.width() - 12 - 29;
  if (m.batteryPercent >= 0) {
    batteryIcon(c, x, 18, m.batteryPercent, m.charging, m.foreground);
    x -= 30;
  }
  drawBitmap(c, icons::wifi, x, 16, 2, m.online ? m.foreground : dimColor(m.foreground));
  c.drawFastHLine(12, 52, c.width() - 24, dimColor(m.foreground));
}

static void row(lgfx::LGFX_Sprite& c, const ScreenModel& m, int y, const Bitmap& icon, const char* text) {
  drawBitmap(c, icon, 12, y, 3, m.foreground);
  c.setTextSize(3);
  fitString(c, text, 60, y + 6, c.width() - 60 - 8);
}

static void drawIdle(lgfx::LGFX_Sprite& c, const ScreenModel& m) {
  face.draw(c, m.foreground, m.blink);
  c.setTextSize(5);
  c.setTextDatum(top_center);
  c.drawString(m.clock, c.width() / 2, 182);
  c.setTextDatum(top_left);
}

static void drawDashboard(lgfx::LGFX_Sprite& c, const ScreenModel& m) {
  header(c, m);
  const Bitmap& wicon = weatherIcon(m.weatherCode);
  if (!isnan(m.temperature)) {
    char t[8];
    snprintf(t, sizeof(t), "%.0f", m.temperature);
    drawBitmap(c, wicon, 12, 64, 3, m.foreground);
    c.setTextSize(3);
    c.drawString(t, 60, 70);
    drawBitmap(c, icons::degree, 60 + (int)strlen(t) * 18 + 2, 70, 3, m.foreground);
  } else {
    row(c, m, 64, wicon, m.weather);
  }
  row(c, m, 120, icons::calendar, m.calendar);
  row(c, m, 176, icons::home, m.home);
}

static void drawMessage(lgfx::LGFX_Sprite& c, const ScreenModel& m) {
  header(c, m);
  drawBitmap(c, icons::mail, 12, 64, 3, m.foreground);
  c.setTextSize(3);
  // Word wrap, 14 chars per line (hard break for long words), up to 5 lines.
  const size_t cols = 14;
  const char* p = m.message;
  for (int line = 0; line < 5 && *p; line++) {
    size_t n = strlen(p);
    if (n > cols) {
      n = cols;
      while (n > 0 && p[n] != ' ') n--;
      if (n == 0) n = cols;
    }
    char buf[cols + 1];
    memcpy(buf, p, n);
    buf[n] = 0;
    c.drawString(buf, 60, 70 + line * 32);
    p += n;
    while (*p == ' ') p++;
  }
}

void drawScreen(lgfx::LGFX_Sprite& c, const ScreenModel& m) {
  c.fillScreen(m.background);
  c.setTextFont(1);
  c.setTextDatum(top_left);
  c.setTextColor(m.foreground, m.background);
  if (m.message && *m.message) drawMessage(c, m);
  else if (m.dashboard) drawDashboard(c, m);
  else drawIdle(c, m);
}
