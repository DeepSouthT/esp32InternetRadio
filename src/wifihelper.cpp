/*******************************
 * wifihelper.cpp
 *
 * Created: 13.09.2020 20:20:00
 * Author : DeepSouthT
 * 
 * Description:
 *     ToDo
 *
 * Last modified: 13.09.2020
 *******************************/

#include "WiFi.h"

#include "wifihelper.h"

bool wifiHelper::connectWifi(const char* ssid, const char* password, unsigned int repetition)
{
    WiFi.begin(ssid, password);

    for(unsigned int i = 0; i < repetition; i++)
    {
        if(WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.println("Not connected");
        } else {
            i = repetition;
            Serial.println("Connected");
        }
    }

    if(WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Disconnecting WiFi");
        WiFi.disconnect();
        delay(2000);
        WiFi.begin(ssid, password);

        for(unsigned int i = 0; i < repetition; i++)
        {
            if(WiFi.status() != WL_CONNECTED)
            {
                delay(500);
                Serial.println("Not connected");
            } else {
                i = repetition;
                Serial.println("Connected");
            }
        }
    }

  if(WiFi.status() == WL_CONNECTED)
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
