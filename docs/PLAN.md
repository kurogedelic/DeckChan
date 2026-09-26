# Implementation plan

## Implemented
- [x] CoreS3 / M5Unified firmware
- [x] Amber bitmap-style console UI
- [x] Selectable amber / green / white / ice palettes
- [x] Idle clock + bitmap face + blink
- [x] Touch dashboard and timed return to idle
- [x] Persistent Preferences-backed configuration
- [x] Local Web Editor
- [x] mDNS at deckchan.local
- [x] Weather adapter (Open-Meteo-compatible JSON)
- [x] Calendar adapter (JSON or plain-text bridge)
- [x] Homebridge adapter (JSON/plain-text + bearer token)
- [x] Manual data refresh endpoint
- [x] Notification endpoint
- [x] Active/idle display brightness
- [x] Scheduled night dimming
- [x] OTA firmware upload endpoint
- [x] Network-failure-safe idle display
- [x] StackChan-BSP integration: head touch (tap/swipe), head nod on wake, RGB LED message glow, battery level
- [x] Web Editor and NTP start on first Wi-Fi connection, not only at boot
- [x] Staggered source refresh (one HTTP fetch per loop pass)
- [x] Host screenshot renderer (`sim/`)

## Hardware validation
The remaining work requires a physical CoreS3/Stack-chan and real service endpoints rather than more speculative firmware:
- [x] Compile against pioarduino (arduino-esp32 3.x) CoreS3 board package
- [ ] Verify upload and boot on the physical unit
- [ ] Tune LCD brightness on the physical unit
- [ ] Tune touch behavior in the Stack-chan enclosure
- [ ] Tune nod angle/speed and servo zero positions (BSP HomeCalibration example)
- [ ] Add authentication to `/update` and `/api/config`
- [ ] Validate chosen Homebridge endpoint/plugin
- [ ] Validate calendar bridge/auth choice

## API
- GET /api/status
- GET /api/config
- POST /api/config
- POST /api/refresh
- POST /api/notify (plain text)
- POST /update (firmware binary)
