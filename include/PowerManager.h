#pragma once
class PowerManager {
public:
  void begin();
  void loop(bool idle);
  bool isNight() const { return night; }
private:
  int applied = -1;
  bool night = false;
};
extern PowerManager powerManager;
