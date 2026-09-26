#pragma once
class WebEditor {
public:
  void begin();
  void loop();
  bool isStarted() const { return started; }
private:
  bool started = false;
};
