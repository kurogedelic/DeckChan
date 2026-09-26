#pragma once
#include <Arduino.h>

struct DeckData {
  String weather = "--";
  String calendar = "NO DATA";
  String home = "NO DATA";
  String message = "";
  uint32_t messageUntil = 0;
};

class DataHub {
public:
  void begin();
  void loop();
  void refresh();  // queues a refresh; sources are fetched one per loop()
  void notify(const String& text, uint32_t seconds = 15);
  const DeckData& data() const { return state; }
private:
  DeckData state;
  uint32_t lastRefresh = 0;
  uint8_t pending = 0;  // next source to fetch, 0 = idle
  String httpGet(const String& url, const String& auth = "");
  void fetchWeather();
  void fetchCalendar();
  void fetchHomebridge();
};

extern DataHub dataHub;
