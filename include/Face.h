#pragma once
#include <M5GFX.h>

class Face {
public:
  static constexpr int width = 56, height = 48;
  void draw(lgfx::LGFX_Sprite& canvas, int x, int y, uint16_t color, bool blink = false);
};
