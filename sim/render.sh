#!/bin/sh
# Renders DeckChan screens to docs/screens/*.png on a Linux/macOS host.
# Needs: g++, SDL2 headers (M5GFX host backend), python3 + Pillow.
set -e
cd "$(dirname "$0")/.."
GFX=${M5GFX_DIR:-.pio/sim/M5GFX}
[ -d "$GFX" ] || git clone -q --depth 1 https://github.com/m5stack/M5GFX "$GFX"
SRCS=$(find "$GFX/src" -name '*.cpp' -o -name '*.c' | grep -v -E 'platforms/(esp|rp2|samd|stm|spres|arduino|opencv|frame)')
mkdir -p .pio/sim/out docs/screens
g++ -std=c++17 -O1 -DLGFX_SDL -I"$GFX/src" -Iinclude sim/main.cpp src/Screen.cpp src/Face.cpp $SRCS \
  $(pkg-config --libs sdl2 2>/dev/null || echo -lSDL2) -lpthread -o .pio/sim/deckchan-sim
.pio/sim/deckchan-sim .pio/sim/out
python3 - <<'PY'
import glob, os
from PIL import Image
for f in glob.glob('.pio/sim/out/*.ppm'):
    name = os.path.splitext(os.path.basename(f))[0]
    Image.open(f).resize((640, 480), Image.NEAREST).save(f'docs/screens/{name}.png')
PY
echo "wrote docs/screens/"
