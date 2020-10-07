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
 * Last modified: 01.10.2020
 *******************************/

#include <Arduino.h>

#include "wifihelper.h"
#include "sdhelper.h"

wifiHelper objWifiHelper;
sdHelper objSdHelper;

void setup()
{
  Serial.begin(115200);
  Serial.println("--------------------------");
  Serial.println("esp32InternetRadio started");
  Serial.println("--------------------------");

  // ----- Get the SSID and PSW -----
  Serial.println("Reading SD card");
  bool status = objSdHelper.initSD();
  if (!status)
  {
    Serial.println("SD not mounted");
  }

  String ssid, pwd;

  status = objSdHelper.readCred(ssid, pwd);
  if (!status)
  {
    Serial.println("Read failed");
  }
  // --------------------------------

  // ----- Connecting to WiFi -------
  Serial.println("Connecting to WiFi");

  Serial.println(ssid);
  Serial.println(pwd);

  status = objWifiHelper.connectWifi(ssid, pwd, 5);

  if (status)
  {
    Serial.println("WiFi connected");
  }
  else
  {
    Serial.println("WiFi not connected");
  }
  // --------------------------------

  objWifiHelper.disconnectWifi();
  objSdHelper.closeSD();
}

void loop()
{
  // put your main code here, to run repeatedly:
}