#pragma once
#include <M5GFX.h>

// Stack-chan face (m5stack-avatar geometry) rendered as 4 px pixel art.
class Face {
public:
  void draw(lgfx::LGFX_Sprite& canvas, uint16_t color, bool blink = false);
};
