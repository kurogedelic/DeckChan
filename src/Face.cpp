#include "Face.h"

void Face::draw(lgfx::LGFX_Sprite& c, int x, int y, uint16_t color, bool blink) {
  // Deliberately blocky: 8 px grid, no anti-aliasing.
  const int p = 8;
  if (blink) {
    c.fillRect(x, y + p, p * 2, p / 2, color);
    c.fillRect(x + p * 5, y + p, p * 2, p / 2, color);
  } else {
    c.fillRect(x, y, p * 2, p * 2, color);
    c.fillRect(x + p * 5, y, p * 2, p * 2, color);
  }
  c.fillRect(x + p * 2, y + p * 4, p, p, color);
  c.fillRect(x + p * 3, y + p * 5, p * 3, p, color);
  c.fillRect(x + p * 6, y + p * 4, p, p, color);
}
