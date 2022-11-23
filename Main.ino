#include "Header.h"
void setup()
{
    Serial.begin(115200);
    rf.config();
}

void loop()
{
    Serial.println("......");
    delay(100);
    rf.hab();
    // rf.sendRFraw("100,0", 12, 2); // example of send rf
    delay(500);

    Serial.println(rf.get());

    delay(5000);
}