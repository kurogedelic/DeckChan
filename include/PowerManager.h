#pragma once
class PowerManager {
public:
  void begin();
  void loop(bool idle);
private:
  bool dimmed = false;
};
extern PowerManager powerManager;
