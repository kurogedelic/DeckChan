# DeckChan

A retro desktop information hub for M5StackChan CoreS3.

DeckChan turns Stack-chan into a quiet always-available desk console: a bitmap face and clock while idle, and a compact dashboard when touched or when something needs attention.

## v0.1 scope

- Amber bitmap UI with selectable palettes
- Idle screen: clock + bitmap face
- Dashboard cards
- Local Web Editor at `deckchan.local`
- Persistent JSON configuration
- Data-source boundary for Weather, Calendar and Homebridge
- Touch to wake the dashboard; automatic return to idle

## Design rules

1. Idle is the primary screen. It should feel alive, not busy.
2. No configuration-heavy UI on the device. Edit from a browser.
3. Information is glanceable. Avoid phone-like app screens.
4. Widgets and data sources stay independent.
5. Amber is the default, never the only palette.
6. Network failure must not break the clock or face.

## Hardware

Target: M5StackChan CoreS3 / CoreS3.

## Build

This repository starts with a PlatformIO/Arduino prototype using M5Unified and M5GFX.

```sh
pio run
pio run -t upload
pio device monitor
```

Copy `include/secrets.example.h` to `include/secrets.h` and set Wi-Fi credentials.

## Roadmap

See [docs/PLAN.md](docs/PLAN.md) and [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md).
