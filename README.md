# AtomSat25 🚀

This is a dual-system IoT project consisting of:

- 📡 `nanosatellite/` — firmware for the satellite (ESP32-based)
- 🛰️ `ground_station/` — firmware for the ground station (ESP32-based)

Each folder is a PlatformIO project. Navigate into the one you want to build, modify, or upload.

## How to Use

```bash
cd nanosatellite
pio run --target upload

