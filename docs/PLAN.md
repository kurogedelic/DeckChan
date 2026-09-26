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

## Hardware validation
The remaining work requires a physical CoreS3/Stack-chan and real service endpoints rather than more speculative firmware:
- [ ] Verify compile/upload against the exact installed PlatformIO CoreS3 board package
- [ ] Tune LCD brightness on the physical unit
- [ ] Tune touch behavior in the Stack-chan enclosure
- [ ] Add servo movement after confirming the servo board/pin mapping in the user's Stack-chan
- [ ] Validate chosen Homebridge endpoint/plugin
- [ ] Validate calendar bridge/auth choice

## API
- GET /api/status
- GET /api/config
- POST /api/config
- POST /api/refresh
- POST /api/notify (plain text)
- POST /update (firmware binary)
