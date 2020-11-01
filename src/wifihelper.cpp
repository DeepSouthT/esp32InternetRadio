/*******************************
 * wifihelper.cpp
 *
 * Created: 13.09.2020 20:20:00
 * Author : DeepSouthT
 * 
 * Description:
 *     ToDo
 *
 * Last modified: 01.11.2020
 *******************************/

#include "WiFi.h"

#include "wifihelper.h"

WiFiClient client;

bool wifiHelper::connectWifi(String ssid, String password, unsigned int repetition)
{
    WiFi.begin((const char *)ssid.c_str(), (const char *)password.c_str());

    for (unsigned int i = 0; i < repetition; i++)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.println("Not connected");
        }
        else
        {
            i = repetition;
            Serial.println("Connected");
        }
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Disconnecting WiFi");
        WiFi.disconnect();
        delay(2000);
        WiFi.begin((const char *)ssid.c_str(), (const char *)password.c_str());

        for (unsigned int i = 0; i < repetition; i++)
        {
            if (WiFi.status() != WL_CONNECTED)
            {
                delay(500);
                Serial.println("Not connected");
            }
            else
            {
                i = repetition;
                Serial.println("Connected");
            }
        }
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println(WiFi.localIP());
        return true;
    }

    return false;
}

void wifiHelper::disconnectWifi(void)
{
    Serial.println("Disconnecting WiFi");
    WiFi.disconnect(true);
}

bool wifiHelper::connectClient(void)
{
    if (client.connect(host, port))
    {
        client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");

        return true;
    }
    else
    {
        return false;
    }
}

uint8_t wifiHelper::readStream(uint8_t *mp3buff)
{
    if (client.available() > 0)
    {
        bytesread = client.read(mp3buff, 32);

        if (!client_status_ok_printed)
        {
            Serial.println("Client available");
            client_status_ok_printed = true;
        }
        client_status_nok_printed = false;
        client_status_nok_counter = 0;
    }
    else
    {
        if (!client_status_nok_printed)
        {
            Serial.println("Client not available");
            client_status_nok_printed = true;
        }
        client_status_ok_printed = false;
        client_status_nok_counter++;

        if(client_status_nok_counter > 10000)
        {
            Serial.println("Reconnecting client");
            (void)connectClient();
            client_status_nok_counter = 0;
        }
    }

    return bytesread;
}