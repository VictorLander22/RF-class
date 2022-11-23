
// tx D8 = 15
// rx D6 = 12

unsigned long lastTime = 0;

bool habInterruption = false;

String rfCode = "";

ICACHE_RAM_ATTR void Interruption()
{
    if (habInterruption)
    {
        const unsigned long timeNow = micros();
        const unsigned long time = timeNow - lastTime;

        rfCode += ",";
        rfCode += time;

        if (rfCode.length() > 500)
            habInterruption = false;
    }
    lastTime = micros();
}
class RF
{
protected:
    uint8_t rx;
    uint8_t tx;

public:
    // Constructor
    RF(uint8_t RX, uint8_t TX)
    {
        rx = RX;
        tx = TX;
    }

    /**
     * @brief
     * Input and output config
     */
    void config()
    {
        pinMode(rx, INPUT);
        attachInterrupt(digitalPinToInterrupt(rx), Interruption, CHANGE);
        pinMode(tx, OUTPUT);
    }

    void sendRFraw(String code, uint16_t size, uint8_t tries)
    {
        if (tries <= 0)
            tries = 2;

        if (size < 2)
            size = 2;

        bool outValue;
        char *str = (char *)code.c_str();
        int arr[size];
        char *p = strtok(str, ",{}  ");
        size_t index = 0;

        Serial.println();

        while (p != nullptr && index < size)
        {
            arr[index++] = atoi(p);
            p = strtok(NULL, ",{} ");
        }

        for (uint8_t i = 0; i < tries; i++)
        {

            for (uint8_t i = 0; i < index; i++)
            {
                if (i == 0)
                {
                    outValue = arr[i];
                    i++;
                }

                outValue = !outValue;
                digitalWrite(tx, outValue);
                delayMicroseconds(arr[i]);
            }
            digitalWrite(tx, 0);
        }
        Serial.println("Codigo RF enviado");
    }

    void hab()
    {
        lastTime = micros();
        habInterruption = true;
    }

    String get()
    {
        String retString = rfCode;
        rfCode = "";
        return retString;
    }
};
