# AtomSat25 🚀

This is a dual-system IoT project consisting of:

- 📡 `nanosatellite/` — firmware for the satellite (ESP32-based)
- 🛰️ `ground_station/` — firmware for the ground station (ESP32-based)

# This is the project structure:

atomSat25/
├── ground_station/
│   ├── include/
│   │   ├── gs_defs.h              # Global constants and pin definitions
│   │   ├── keypad_handler.h       # Keypad class interface
│   │   ├── lcd_display.h          # LCD class interface
│   │   └── lora_handler.h         # LoRa communication interface
│   ├── src/
│   │   ├── main.cpp               # Primary application logic
│   │   ├── keypad_handler.cpp     # Keypad implementation
│   │   ├── lcd_display.cpp        # LCD implementation
│   │   └── lora_handler.cpp       # LoRa implementation
│   ├── lib/                       # (Empty - using PlatformIO libraries)
│   └── platformio.ini             # Ground station specific config
│
├── nanosatellite/
│   ├── include/
│   │   ├── sat_defs.h             # Satellite-specific constants
│   │   ├── sensor_handler.h       # Sensor interface
│   │   ├── oled_display.h         # OLED interface
│   │   └── lora_handler.h         # LoRa interface
│   ├── src/
│   │   ├── main.cpp               # Main satellite logic
│   │   ├── sensor_handler.cpp     # Sensor implementation
│   │   ├── oled_display.cpp       # OLED implementation
│   │   └── lora_handler.cpp       # LoRa implementation
│   ├── lib/                       # (Empty)
│   └── platformio.ini             # Satellite specific config
│
└── README.md                      # Project documentation


Each folder is a PlatformIO project. Navigate into the one you want to build, modify, or upload.

## How to Use

```bash
cd nanosatellite
pio run --target upload

