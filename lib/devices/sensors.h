#include <Wire.h>
#include "Adafruit_INA219.h"
#include <WebSerial.h>

Adafruit_INA219 ina219;
TwoWire I2CINA = TwoWire(0);

void ina219Setup(int sdaPin, int sdcPin)
{
    Serial.println("** [INA219 Sensor] Init **");

    I2CINA.begin(sdaPin, sdcPin, 100000);
    if (!ina219.begin(&I2CINA))
    {
        Serial.println("Failed to find INA219 chip");
        while (1)
        {
            delay(10);
        }
    }
    // WebSerial.print("BV");
    // WebSerial.print("\t"); // Bus Voltage
    // WebSerial.print("SV");
    // WebSerial.print("\t"); // Shunt Voltage
    // WebSerial.print("LV");
    // WebSerial.print("\t"); // Load Voltage
    // WebSerial.print("C");
    // WebSerial.print("\t");  // Current
    // WebSerial.println("P"); // Power
}

void ina219DisplayValues()
{
    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_mA = 0;
    float loadvoltage = 0;
    float power_mW = 0;

    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);

    // WebSerial.print(busvoltage);
    // WebSerial.print("\t");
    // WebSerial.print(shuntvoltage);
    // WebSerial.print("\t");
    WebSerial.print(loadvoltage);
    WebSerial.print("V\t");
    WebSerial.print(current_mA);
    WebSerial.println("mA");
    // WebSerial.print("\t");
    // WebSerial.println(power_mW);
}