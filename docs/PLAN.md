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

## Next implementation plan

Order is chosen so each phase is usable on its own. UI changes are checked with
`sim/render.sh` screenshots before touching hardware.

### Phase 1 — Hardware bring-up (needs the unit)
1. Upload, confirm boot log: BSP init (IO expander, servos, INA226) and Wi-Fi.
2. Check readability of the 2.0" panel at arm's length; adjust text sizes if needed.
3. Head touch: tune `TouchSensor.setSensitivity`, confirm tap vs swipe.
4. Servo zero positions via the BSP `HomeCalibration` example; tune nod angle/speed.
5. Brightness defaults (active/idle/night) and battery % curve against real readings.

### Phase 2 — UI polish
1. Japanese text: switch message/calendar/home rows to `lgfxJapanGothic_16/24`
   (ASCII stays on the pixel font).
2. Long text: slow horizontal scroll for rows that do not fit (about 13 chars at 3x).
3. Face expressions as bitmaps: sleepy at night, surprised when a message arrives,
   happy on head tap. Mouth/eye variants live next to the default face in `Face.cpp`.
4. Offline and "no time yet" states: clear icon instead of `--:--` only.

### Phase 3 — Data sources
1. Weather: Web Editor takes latitude/longitude and builds the Open-Meteo URL
   (`current=temperature_2m,weather_code`).
2. Calendar: document a tiny bridge contract (`{"next":"10:30 REVIEW"}`) and ship an
   example bridge script (ICS → JSON) under `tools/`.
3. Homebridge: support the homebridge-config-ui-x API (token login, accessory status).
4. Show per-source errors in the Web Editor status (last fetch time, HTTP code).

### Phase 4 — Safety
1. Shared secret (set in `secrets.h`) required for `POST /api/config`, `/api/notify`
   and `/update`; the Web Editor asks for it once and keeps it in the browser.
2. OTA: check `Update.begin/write/end` results and reject non-firmware uploads.

### Phase 5 — Power
1. Night: display off after the idle timeout, wake on touch/head tap.
2. Idle: lower refresh rate for data sources; redraw only when the minute changes.

### Phase 6 — CI
1. GitHub Actions: `pio run` for the firmware and `sim/render.sh` to upload
   screenshots as build artifacts.

## API
- GET /api/status
- GET /api/config
- POST /api/config
- POST /api/refresh
- POST /api/notify (plain text)
- POST /update (firmware binary)
