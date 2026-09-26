# Architecture

DeckChan is split into five layers.

## Display
M5GFX renders a deliberately low-resolution console aesthetic. Idle rendering is independent of network services.

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
