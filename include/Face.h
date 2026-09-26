#pragma once
#include <M5Unified.h>

class Face {
public:
  void draw(M5Canvas& canvas, int x, int y, uint16_t color, bool blink = false);
};
