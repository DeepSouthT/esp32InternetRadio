/*******************************
 * main.c
 *
 * Created: 12.09.2020 20:00:00
 * Author : DeepSouthT
 *
 * Used:
 *    IC > 
 *    IDE > 
 *    Programmer > 
 * 
 * Description:
 *     ToDo
 *
 * Last modified: 28.09.2020
 *******************************/

#include <Arduino.h>

#include "wifihelper.h"
#include "sdhelper.h"

const char* ssid = "";
const char* password =  "";

wifiHelper objWifiHelper;
sdHelper objSdHelper;

void setup() {
  Serial.begin(115200);
  Serial.println("--------------------------");
  Serial.println("esp32InternetRadio started");
  Serial.println("--------------------------");

  Serial.println("Connecting to WiFi");
  bool status = objWifiHelper.connectWifi(ssid, password, 5);

  if (status)
  {
    Serial.println("WiFi connected");
  } else {
    Serial.println("WiFi not connected");
  }

  status = objSdHelper.initSD();

  if (status)
  {
    Serial.println("SD mounted");
  } else {
    Serial.println("SD not mounted");
  }

  objWifiHelper.disconnectWifi();

  objSdHelper.closeSD();
}

void loop() {
  // put your main code here, to run repeatedly:
}