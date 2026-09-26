#include <M5Unified.h>
#include <WiFi.h>
#include <time.h>
#include "DeckConfig.h"
#include "Face.h"
#include "WebEditor.h"
#include "DataHub.h"
#include "PowerManager.h"
#if __has_include("secrets.h")
#include "secrets.h"
#else
#define DECKCHAN_WIFI_SSID ""
#define DECKCHAN_WIFI_PASSWORD ""
#endif

M5Canvas canvas(&M5.Display); Face face; WebEditor web;
uint32_t lastInteraction=0,lastBlink=0; bool blink=false,dashboard=false;

static void labelBox(int x,int y,int w,int h,const String& title,const String& value){
  canvas.drawRect(x,y,w,h,deckConfig.foreground);canvas.setTextSize(1);canvas.drawString(title,x+7,y+7);canvas.setTextSize(2);canvas.drawString(value.substring(0,18),x+7,y+24);
}
static void drawFrame(){
  canvas.fillScreen(deckConfig.background);canvas.setTextColor(deckConfig.foreground,deckConfig.background);canvas.setTextDatum(top_left);canvas.setTextFont(1);
  struct tm t;if(getLocalTime(&t,5)){char s[20];strftime(s,sizeof(s),deckConfig.showSeconds?"%H:%M:%S":"%H:%M",&t);canvas.setTextSize(3);canvas.drawString(s,12,10);}else{canvas.setTextSize(3);canvas.drawString("--:--",12,10);}
  const auto& d=dataHub.data();
  if(d.message.length()){canvas.setTextSize(2);canvas.drawRect(8,72,304,96,deckConfig.foreground);canvas.drawString("MESSAGE",18,82);canvas.setTextSize(1);canvas.drawString(d.message,18,118);}
  else if(!dashboard){face.draw(canvas,128,88,deckConfig.foreground,blink);canvas.setTextSize(1);canvas.drawString("DECKCHAN // IDLE",12,218);}
  else{canvas.setTextSize(1);canvas.drawString("DASHBOARD",12,58);labelBox(12,76,142,60,"WEATHER",d.weather);labelBox(166,76,142,60,"NEXT",d.calendar);labelBox(12,148,296,54,"HOME",d.home);canvas.drawString(WiFi.status()==WL_CONNECTED?"NET ONLINE":"NET OFFLINE",12,218);}
  canvas.pushSprite(0,0);
}
void setup(){
  auto c=M5.config();M5.begin(c);M5.Display.setRotation(1);loadDeckConfig();M5.Display.setBrightness(deckConfig.activeBrightness);canvas.createSprite(M5.Display.width(),M5.Display.height());
  WiFi.mode(WIFI_STA);if(strlen(DECKCHAN_WIFI_SSID)){WiFi.begin(DECKCHAN_WIFI_SSID,DECKCHAN_WIFI_PASSWORD);uint32_t s=millis();while(WiFi.status()!=WL_CONNECTED&&millis()-s<10000)delay(100);}
  if(WiFi.status()==WL_CONNECTED){configTzTime("JST-9","pool.ntp.org","time.google.com");web.begin();}
  dataHub.begin();powerManager.begin();lastInteraction=millis();drawFrame();
}
void loop(){
  M5.update();web.loop();dataHub.loop();auto touch=M5.Touch.getDetail();
  if(touch.wasPressed()){dashboard=true;lastInteraction=millis();drawFrame();}
  if(dashboard&&millis()-lastInteraction>deckConfig.idleAfterMs){dashboard=false;drawFrame();}
  if(!dashboard&&millis()-lastBlink>4500){blink=true;drawFrame();delay(80);blink=false;lastBlink=millis();drawFrame();}
  static uint32_t tick=0;if(millis()-tick>1000){tick=millis();drawFrame();}
  powerManager.loop(!dashboard);delay(10);
}
