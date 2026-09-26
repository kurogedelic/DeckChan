# Architecture

DeckChan is split into five layers.

## Display
M5GFX renders a deliberately low-resolution console aesthetic on the 320x240, 2.0" panel. `Screen.cpp` draws from a plain `ScreenModel` snapshot and has no Arduino dependencies, so `sim/render.sh` can render the same code on a host. The idle face is the m5stack-avatar Stack-chan face as 4 px pixel art (`Face.cpp`); icons are 1-bit bitmaps in `Bitmaps.h`. Idle rendering is independent of network services.

## Hardware
The official StackChan-BSP (`M5StackChan`) provides head touch, servos, body RGB LEDs and battery readings. It requires arduino-esp32 3.x (pioarduino platform).

## Widgets
Small cards render already-normalized state. Widgets never fetch data directly.

Planned built-ins: Clock, Weather, Calendar, Homebridge, System Status, Message and Timer.

## Data sources
Adapters normalize external systems into small structs/JSON objects. Initial targets are Weather, calendar feeds and Homebridge.

## Web Editor
The device serves a local configuration UI. The editor controls palette, idle timeout, widget order and source settings. The device remains usable without the editor.

## Power
Three states are planned:

- ACTIVE: dashboard and normal refresh.
- IDLE: clock + bitmap face, reduced animation and network polling.
- NIGHT: display off / optional deep sleep with RTC wake.

Deep sleep is intentionally not used for normal idle because the persistent clock-and-face screen is part of DeckChan's identity.
