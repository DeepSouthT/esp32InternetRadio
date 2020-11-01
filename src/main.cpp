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
 * Last modified: 20.10.2020
 *******************************/

#include <Arduino.h>

#include <VS1053.h>

#include "wifihelper.h"
#include "sdhelper.h"

#define VS1053_CS 32
#define VS1053_DCS 33
#define VS1053_DREQ 35

#define VOLUME 100 // volume level 0-100

uint8_t bytesread = 0;
uint8_t mp3buff[32]; // vs1053 likes 32 bytes at a time

VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);

sdHelper objSdHelper;
wifiHelper objWifiHelper;

void initMP3Decoder()
{
  player.begin();
  player.switchToMp3Mode(); // optional, some boards require this
  player.setVolume(VOLUME);
}

void setup()
{

  Serial.begin(9600);
  delay(500);
  Serial.println("--------------------------");
  Serial.println("esp32InternetRadio started");
  Serial.println("--------------------------");

  SPI.begin();

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

  objSdHelper.closeSD();
  // --------------------------------

  initMP3Decoder();

  // ----- Connecting to WiFi -------
  Serial.println("Connecting to WiFi");

  Serial.println(ssid);
  Serial.println(pwd);

  status = objWifiHelper.connectWifi(ssid, pwd, 2);
  if (status)
  {
    Serial.println("WiFi connected");
  }
  else
  {
    Serial.println("WiFi not connected");
  }
  // --------------------------------

  // ----- Connecting to Station/Client -------
  status = objWifiHelper.connectClient();
  if (status)
  {
    Serial.println("Client connected");
  }
  else
  {
    Serial.println("Client not connected");
  }
  // --------------------------------
}

void loop()
{

  bytesread = objWifiHelper.readStream(mp3buff);

  player.playChunk(mp3buff, bytesread);
}
