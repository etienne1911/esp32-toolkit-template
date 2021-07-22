# ESP32 Toolkit Template

Aiming to be used as a base for new projects or sandbox...

## Embedded Features
- wifi network: AP + STA
- internal filesystem: LittleFS
- web services: Webserver, Websockets 
- OTA updates
- power management: sleep/wake cycle

## Prereq

VSCode + PlatformIO extension are required.

In platform.ini `board` and `monitor_speed`  can be customized.

 ## Configuration

### Network settings

Both AP (access point) and STA (wifi client) modes are supported and can work in the same time.

Before building and uploading, configure wifi settings (SSID + password) for each modes.

Once the board boots, the registered IP address will be displayed in the serial console.

### OTA updates

This feature permits to update device over-the-air without resorting to serial transfer.

Note that you must first use classic method using serial communications before you can flash using OTA.

Also make sure wifi settings are correctly setup in previous step to allow device to connect to the WLAN network.

After building a new firmware, a `firmware.bin` file will be output in `.pio/build/<board-model>/firmware.bin` folder of the project.

While device is booted and connected to your WLAN network, browse the following url `<deviceIP>/update` which should display an updater page.
Select the firmware file that was output before to upload it to your board.
Now the device should be updated with the latest version of your code

Using the same method, you can also update device's internal storage(see Filesystem step).
Simply choose `filesystem` option instead of `firmware` on the updater page, and select the filesystem image which will be available in `.pio/build/<board-model>/SPIFFS.bin` after building in the next step.

### Filesystem

The board is able to handle a small filesystem (LittleFS) to store files.
This will be particularly handy when serving web pages through webserver (described at next step).
To use it you'll need to put the files in the `data` folder of the project and build a filesystem image, 
by running `<boardname> -> Platform -> Build Filesystem Image` task.
This will output a `SPIFFS.bin` image in the `.pio/build/<board-model>/`, that you can then upload the the board either through serial or OTA updater.

### Web Server

To host static web pages, include the files you want to serve in the filesystem image from previous step.
Upload the filesystem image to your device, and access each page by browsing file's url: `<deviceIp>/filename` 

### Web Sockets

Web socket protocol is also available on the board. 
It is much lighter and faster than common http requests and particularly useful in remote control application where real-time connection are needed.

### Power Management

Coming soon.

## Credits

This template was mainly inspired from this [tutorial](https://randomnerdtutorials.com/esp32-ota-over-the-air-arduino/).