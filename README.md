# ESP32 Toolkit Template

Embedd most common features for the ESP32 boards to quick start new projects.

## Overview
- wifi: support AP + STA modes
- internal filesystem: LittleFS
- web services: Webserver, Websockets 
- OTA updates
- devices support: oled screen, current sensor, digpot 
- power management: TODO sleep/wake cycle

## Usage

VSCode + PlatformIO extension are required.

In platform.ini `board` and `monitor_speed`  can be customized.

 ## Features

### Network

Both AP (access point) and STA (wifi client) modes are supported and can work at the same time.

Wifi settings (SSID + password) set in external file

Once the board boots, the registered IP address will be displayed in the serial console.

### OTA updates

Over-the-air device firmware updater access at: `<deviceIP>/update` 

without resorting to serial transfer.

Note: firmware must be uploaded using serial communications the first time before OTA flash is available.

Note2: wifi settings must be correctly setup to allow device connection to the WLAN network.

On each new firmware build, upload `firmware.bin` from `.pio/build/<board-model>/firmware.bin` project's folder.

Same method, for updating device's internal storage (see Filesystem step) by choosing `filesystem` option (instead of `firmware`) in OTA updater
and selecting filesystem image built at next step.

### Filesystem

Support for LittleFS filesystem.
Files to include in filestystem must be put in project's `data` folder.
Build a new filesystem image, by running `<boardname> -> Platform -> Build Filesystem Image` task.
This will output a `SPIFFS.bin` image in the `.pio/build/<board-model>/`, that can then be uploaded the the board either through serial or OTA updater.

### Web Server

Static web pages server, by include web files in the filesystem image described at previous step.
Page access through following url: `<deviceIP>/filename` 

### Web Sockets

Web socket protocol to provide much lighter and faster communication with device than common http requests 
( particularly useful for realtime remote control applications ).

### Loggin

coming soon

### Devices support
- SSD1306 oled display
- INA219 current sensor
- X9C104 digital potentiometer

### Power Management

TODO

