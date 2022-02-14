// #include <Arduino.h>
#include <DigiPotX9Cxxx.h>

#define POT_MAX_VAL 16
#define POT_MIN_VAL 4

int target = POT_MAX_VAL;
int timer = 60; // seconds
int startTimer = 5;

/**
 * Digipot X9C104 
 */
DigiPot pot(12, 13, 2); // inc, ud, cs

void test()
{
    float voltage;

    for (int i = 0; i < 100; i++)
    {
        pot.increase(1);
        voltage = 5.0 * analogRead(A0) / 1024;
        Serial.println(voltage);
        delay(20);
    }

    for (int i = 0; i < 100; i++)
    {
        pot.decrease(1);
        voltage = 5.0 * analogRead(A0) / 1024;
        Serial.println(voltage);
        delay(20);
    }
}

void resetPot(int val = POT_MAX_VAL)
{
    target = val;
    pot.set(target);
}

void cycle()
{
    int current = pot.get();
    // UP or DOWN cycle phase
    if (current != target)
    {
        if (startTimer == 0)
        {
            current < target ? pot.increase(1) : pot.decrease(1);
            WebSerial.print(current < target ? "going down " : "ramp up ");
            WebSerial.println(current);
            delay(1000);
        }
        else
        {
            if (startTimer == 5)
                WebSerial.print("Starting in ");
            WebSerial.println(startTimer--);
        }
    }
    // half: UP cycle done
    if (current == POT_MIN_VAL && timer > 0)
    {
        WebSerial.print("remaining time: ");
        WebSerial.println(timer--);
        if (timer == 0)
        {
            WebSerial.println("UP cycle done, start DOWN cycle ");
            target = POT_MAX_VAL;
        }
        delay(1000);
    }
    // whole cycle completed
    if (current == POT_MAX_VAL && startTimer <= 0 && timer <= 0)
    {
        WebSerial.println("Whole cycle completed! ");
        // reset timers
        timer = 60;
        startTimer = 5;
    }
    // pot.set(val);
}

void digiPotLoop()
{
    // int voltage = 5.0 * analogRead(A0) / 1024;
    // Serial.println(voltage);
    WebSerial.println("Next cycle in ...");
    delay(5000);
}