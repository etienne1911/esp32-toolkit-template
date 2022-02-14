#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WebSerial.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

class Display
{
public:
    // static Display *instance;

    TwoWire I2CInterface = TwoWire(0);

    Adafruit_SSD1306 *display; // = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &I2CInterface, -1);

    Display(int sdaPin, int sdcPin, int i2cAddr)
    {
        // Wire.begin(sdaPin, sdcPin);
        WebSerial.println("** [Display] OLED Screen **");
        //   Serial.begin(115200);
        I2CInterface.begin(sdaPin, sdcPin, 100000);
        display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &I2CInterface, -1);
        WebSerial.println("** [Display] DBG #1 **");
        if (!display->begin(SSD1306_SWITCHCAPVCC, i2cAddr))
        { // Address 0x3D for 128x64
            WebSerial.println(F("[Display] [ERR] SSD1306 allocation failed"));
            for (;;)
                ;
        }
    }

    void testDisplay()
    {
        WebSerial.println("** [Display] Hello World Test **");
        // delay(2000);
        display->clearDisplay();

        display->setTextSize(1);
        display->setTextColor(WHITE);
        display->setCursor(0, 0);
        // Display static text
        display->println("Hello, world!");
        display->display();
        delay(100);
    }
};