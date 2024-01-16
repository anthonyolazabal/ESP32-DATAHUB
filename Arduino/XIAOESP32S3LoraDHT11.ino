#include <Arduino.h>
#include "DHT.h"
#include "Wire.h"

#define DHTPIN A0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#if defined(ARDUINO_ARCH_AVR)
#define debug Serial
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM)
#define debug SerialUSB
#else
#define debug Serial
#endif

static char recv_buf[512];
static bool is_exist = false;
static bool is_join = false;
static int led = 0;

static int at_send_check_response(char *p_ack, int timeout_ms, char *p_cmd, ...)
{
    int ch;
    int num = 0;
    int index = 0;
    int startMillis = 0;
    va_list args;
    memset(recv_buf, 0, sizeof(recv_buf));
    va_start(args, p_cmd);
    Serial1.printf(p_cmd, args);
    Serial.printf(p_cmd, args);
    va_end(args);
    delay(200);
    startMillis = millis();

    if (p_ack == NULL)
    {
        return 0;
    }

    do
    {
        while (Serial1.available() > 0)
        {
            ch = Serial1.read();
            recv_buf[index++] = ch;
            Serial.print((char)ch);
            delay(2);
        }

        if (strstr(recv_buf, p_ack) != NULL)
        {
            return 1;
        }

    } while (millis() - startMillis < timeout_ms);
    return 0;
}

static void recv_prase(char *p_msg)
{
    if (p_msg == NULL)
    {
        return;
    }
    char *p_start = NULL;
    int data = 0;
    int rssi = 0;
    int snr = 0;

    p_start = strstr(p_msg, "RX");
    if (p_start && (1 == sscanf(p_start, "RX: \"%d\"\r\n", &data)))
    {
        Serial.println(data);
        led = !!data;
        if (led)
        {
            digitalWrite(LED_BUILTIN, LOW);
        }
        else
        {
            digitalWrite(LED_BUILTIN, HIGH);
        }
    }

    p_start = strstr(p_msg, "RSSI");
    if (p_start && (1 == sscanf(p_start, "RSSI %d,", &rssi)))
    {
        Serial.println("rssi:");
        Serial.println(rssi);
    }
    p_start = strstr(p_msg, "SNR");
    if (p_start && (1 == sscanf(p_start, "SNR %d", &snr)))
    {
        Serial.println("snr :");
        Serial.println(snr);
    }
}

void setup(void)
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.print("HIVEMQ ESP32 LORAWAN LAB\r\n");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial1.begin(9600, SERIAL_8N1, 44, 43);

    // Init Wio-E5
    if (at_send_check_response("+AT: OK", 100, "AT\r\n"))
    {
        is_exist = true;
        at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
        at_send_check_response("+MODE: LWOTAA", 1000, "AT+MODE=LWOTAA\r\n");                                    // select mode
        at_send_check_response("+DR: EU868", 1000, "AT+DR=EU868\r\n");                                          // select frequency band
        at_send_check_response("+CH: NUM", 1000, "AT+CH=NUM,0-2\r\n");                                          // select sub band
        at_send_check_response("+ID: DevEui", 1000, "AT+ID=DevEui,\"2CF7F1C042900211\"\r\n");                   // write deveui
        at_send_check_response("+ID: AppEui", 1000, "AT+ID=AppEui,\"0128944F83E7AF55\"\r\n");                   // write AppEui
        at_send_check_response("+KEY: APPKEY", 1000, "AT+KEY=APPKEY,\"35d313e21071d5f41f651da500712677\"\r\n"); // write appkey
        at_send_check_response("+CLASS: C", 1000, "AT+CLASS=A\r\n");                                            // write device class
        at_send_check_response("+PORT: 8", 1000, "AT+PORT=8\r\n");
        delay(200);
        is_join = true;
    }
    else
    {
        is_exist = false;
        Serial.print("No Wio-E5 module found.\r\n");
    }

    dht.begin();
}

void loop(void)
{
    float temp = 0;
    float humi = 0;

    temp = dht.readTemperature();
    humi = dht.readHumidity();

    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" *C");

    if (is_exist)
    {
        int ret = 0;
        if (is_join)
        {

            ret = at_send_check_response("+JOIN: Network joined", 12000, "AT+JOIN\r\n");
            if (ret)
            {
                is_join = false;
            }
            else
            {
                at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
                Serial.print("JOIN failed!\r\n\r\n");
                delay(5000);
            }
        }
        else
        {
            char cmd[128];
            sprintf(cmd, "AT+CMSG=\"T:%d-H:%d\"\r\n", (int)temp, (int)humi);
            ret = at_send_check_response("Done", 5000, cmd);
            if (ret)
            {
                recv_prase(recv_buf);
            }
            else
            {
                Serial.print("Send failed!\r\n\r\n");
            }
            delay(5000);
        }
    }
    else
    {
        delay(1000);
    }
}
