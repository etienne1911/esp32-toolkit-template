#include <WebSerial.h>
#include <digpot.h>
// #include <Display.h>

void recvMsg(uint8_t *data, size_t len)
{
    WebSerial.println("Received Data...");
    String d = "";
    for (int i = 0; i < len; i++)
    {
        d += char(data[i]);
    }
    WebSerial.println(d);
    if (d == "DOWN")
    {
        // digitalWrite(LED, HIGH);
        // setupOledTest();
        // Display disp(13, 12, 0x3C);
        WebSerial.println("DOWN cycle");
        // test();
        // cycle(10, 20, 500, 30);
        target = POT_MAX_VAL;
    }
    if (d == "UP")
    {
        // digitalWrite(LED, LOW);
        WebSerial.println("UP cycle");
        target = POT_MIN_VAL;
    }
}