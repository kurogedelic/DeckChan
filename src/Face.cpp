#include "Face.h"
#include "Bitmaps.h"

// m5stack-avatar default face on 320x240: eyes r=8 (drawn 20 px so they read round in pixels) at (90,93) and (230,96),
// mouth 50..90 x 4 centred at (163,148). Snapped to a 4 px grid here.
static constexpr int P = 4;
static constexpr Bitmap eyeOpen{5, 5,
  ".###."
  "#####"
  "#####"
  "#####"
  ".###."};
static constexpr Bitmap eyeClosed{5, 1, "#####"};
static constexpr Bitmap mouth{14, 1, "##############"};

void Face::draw(lgfx::LGFX_Sprite& c, uint16_t color, bool blink) {
  const Bitmap& eye = blink ? eyeClosed : eyeOpen;
  int dy = blink ? P * 2 : 0;  // closed eye sits on the eye's centre row
  drawBitmap(c, eye, 90 - 10, 93 - 10 + dy, P, color);
  drawBitmap(c, eye, 230 - 10, 96 - 10 + dy, P, color);
  drawBitmap(c, mouth, 163 - 7 * P, 148 - P / 2, P, color);
}
