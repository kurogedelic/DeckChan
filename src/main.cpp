#include <M5Unified.h>
#include <WiFi.h>
#include <time.h>
#include "DeckConfig.h"
#include "Face.h"
#include "WebEditor.h"

#if __has_include("secrets.h")
#include "secrets.h"
#else
#define DECKCHAN_WIFI_SSID ""
#define DECKCHAN_WIFI_PASSWORD ""
#endif

M5Canvas canvas(&M5.Display);
Face face;
WebEditor web;
uint32_t lastInteraction = 0;
uint32_t lastBlink = 0;
bool blink = false;
bool dashboard = false;

static void drawFrame() {
  canvas.fillScreen(deckConfig.background);
  canvas.setTextColor(deckConfig.foreground, deckConfig.background);
  canvas.setTextDatum(top_left);
  canvas.setTextFont(1);

  struct tm t;
  if (getLocalTime(&t, 10)) {
    char clockText[16];
    strftime(clockText, sizeof(clockText), deckConfig.showSeconds ? "%H:%M:%S" : "%H:%M", &t);
    canvas.setTextSize(3);
    canvas.drawString(clockText, 12, 10);
  } else {
    canvas.setTextSize(2);
    canvas.drawString("--:--", 12, 10);
  }

  if (!dashboard) {
    face.draw(canvas, 128, 88, deckConfig.foreground, blink);
    canvas.setTextSize(1);
    canvas.drawString("DECKCHAN // IDLE", 12, 218);
  } else {
    canvas.setTextSize(1);
    canvas.drawString("DASHBOARD", 12, 60);
    canvas.drawRect(12, 82, 142, 56, deckConfig.foreground);
    canvas.drawString("WEATHER", 20, 90);
    canvas.drawString("-- C", 20, 112);
    canvas.drawRect(166, 82, 142, 56, deckConfig.foreground);
    canvas.drawString("NEXT", 174, 90);
    canvas.drawString("NO DATA", 174, 112);
    canvas.drawRect(12, 150, 296, 50, deckConfig.foreground);
    canvas.drawString("HOME  //  WAITING FOR SOURCE", 20, 166);
  }
  canvas.pushSprite(0, 0);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.setRotation(1);
  canvas.createSprite(M5.Display.width(), M5.Display.height());

  WiFi.mode(WIFI_STA);
  if (strlen(DECKCHAN_WIFI_SSID)) {
    WiFi.begin(DECKCHAN_WIFI_SSID, DECKCHAN_WIFI_PASSWORD);
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 8000) delay(100);
  }
  if (WiFi.status() == WL_CONNECTED) {
    configTzTime("JST-9", "pool.ntp.org", "time.google.com");
    web.begin();
  }

  lastInteraction = millis();
  drawFrame();
}

void loop() {
  M5.update();
  web.loop();

  auto touch = M5.Touch.getDetail();
  if (touch.wasPressed()) {
    dashboard = true;
    lastInteraction = millis();
    drawFrame();
  }

  if (dashboard && millis() - lastInteraction > deckConfig.idleAfterMs) {
    dashboard = false;
    drawFrame();
  }

  if (!dashboard && millis() - lastBlink > 4500) {
    blink = true;
    drawFrame();
    delay(90);
    blink = false;
    lastBlink = millis();
    drawFrame();
  }

  static uint32_t lastClock = 0;
  if (millis() - lastClock > 1000) {
    lastClock = millis();
    drawFrame();
  }
  delay(10);
}
