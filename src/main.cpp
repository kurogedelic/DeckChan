#include <M5StackChan.h>
#include <WiFi.h>
#include <time.h>
#include "DeckConfig.h"
#include "Screen.h"
#include "WebEditor.h"
#include "DataHub.h"
#include "PowerManager.h"
#if __has_include("secrets.h")
#include "secrets.h"
#else
#define DECKCHAN_WIFI_SSID ""
#define DECKCHAN_WIFI_PASSWORD ""
#endif

M5Canvas canvas(&M5.Display); WebEditor web;
uint32_t lastInteraction=0,lastBlink=0,blinkUntil=0,lastDraw=0; bool dashboard=false,messageShown=false;

static int batteryPercent(float v){ return v<1.0f?-1:constrain((int)((v-3.3f)/(4.15f-3.3f)*100),0,100); }

static void drawFrame(){
  char clock[9]="--:--",date[16]=""; struct tm t;
  if(getLocalTime(&t,5)){strftime(clock,sizeof(clock),deckConfig.showSeconds?"%H:%M:%S":"%H:%M",&t);strftime(date,sizeof(date),"%a %m/%d",&t);for(char* p=date;*p;p++)*p=toupper(*p);}
  const auto& d=dataHub.data(); ScreenModel m;
  m.clock=clock; m.date=date; m.weather=d.weather.c_str(); m.calendar=d.calendar.c_str(); m.home=d.home.c_str(); m.message=d.message.c_str();
  m.dashboard=dashboard; m.blink=millis()<blinkUntil; m.online=WiFi.status()==WL_CONNECTED;
  m.batteryPercent=batteryPercent(M5StackChan.getBatteryVoltage()); m.charging=M5StackChan.getBatteryCurrent()<-0.01f;
  m.foreground=deckConfig.foreground; m.background=deckConfig.background;
  drawScreen(canvas,m); canvas.pushSprite(0,0); lastDraw=millis();
}
static bool motionAllowed(){ return deckConfig.motionEnabled&&!powerManager.isNight(); }
static void setDashboard(bool on){
  if(on) lastInteraction=millis();
  if(on==dashboard) return;
  dashboard=on;
  if(motionAllowed()){ if(on) M5StackChan.Motion.move(0,150,300); else M5StackChan.Motion.goHome(200); }
  drawFrame();
}
static void updateLeds(){
  bool show=dataHub.data().message.length()>0;
  if(show==messageShown) return;
  messageShown=show; uint16_t c=deckConfig.foreground;
  // RGB565 -> 8-bit, kept at quarter brightness so the body glows rather than glares.
  if(show&&deckConfig.ledsEnabled) M5StackChan.showRgbColor(((c>>11)&31)*2,((c>>5)&63),(c&31)*2); else M5StackChan.showRgbColor(0,0,0);
  if(show) lastInteraction=millis();
}
static void networkLoop(){
  if(WiFi.status()!=WL_CONNECTED||web.isStarted()) return;
  // Runs once, on the first connection — even if Wi-Fi was down at boot.
  configTzTime("JST-9","pool.ntp.org","time.google.com"); web.begin(); dataHub.refresh();
}
void setup(){
  M5StackChan.begin(); M5.Display.setRotation(1); loadDeckConfig(); M5.Display.setBrightness(deckConfig.activeBrightness);
  canvas.createSprite(M5.Display.width(),M5.Display.height());
  M5StackChan.Motion.setAutoTorqueReleaseEnabled(true);
  WiFi.mode(WIFI_STA); WiFi.setAutoReconnect(true);
  if(strlen(DECKCHAN_WIFI_SSID)){WiFi.begin(DECKCHAN_WIFI_SSID,DECKCHAN_WIFI_PASSWORD);uint32_t s=millis();while(WiFi.status()!=WL_CONNECTED&&millis()-s<10000)delay(100);}
  networkLoop(); dataHub.begin(); powerManager.begin(); lastInteraction=millis(); drawFrame();
}
void loop(){
  M5StackChan.update(); networkLoop(); web.loop(); dataHub.loop(); updateLeds();
  auto& head=M5StackChan.TouchSensor;
  if(M5.Touch.getDetail().wasPressed()||head.wasClicked()) setDashboard(true);
  if(head.wasSwipedForward()) setDashboard(true);
  if(head.wasSwipedBackward()) setDashboard(false);
  if(dashboard&&millis()-lastInteraction>deckConfig.idleAfterMs) setDashboard(false);
  if(!dashboard&&millis()-lastBlink>4500){ lastBlink=millis(); blinkUntil=lastBlink+80; drawFrame(); }
  if(blinkUntil&&millis()>=blinkUntil){ blinkUntil=0; drawFrame(); }
  if(millis()-lastDraw>1000) drawFrame();
  powerManager.loop(!dashboard); delay(10);
}
