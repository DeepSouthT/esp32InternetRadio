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
 * Last modified: 15.12.2020
 *******************************/

#include <Arduino.h>

#include <VS1053.h>

#include "wifihelper.h"
#include "sdhelper.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint8_t total_aval_stations = 0;
uint8_t initial_station = 0;

const int buttonUp = 16;     // the number of the pushbutton pin
const int buttonDown = 17;     // the number of the pushbutton pin
int buttonUpState = 0;         // variable for reading the pushbutton status
int buttonDownState = 0;         // variable for reading the pushbutton status
int buttonUpStateOld = 0;         // variable for reading the pushbutton status
int buttonDownStateOld = 0;         // variable for reading the pushbutton status

#define VS1053_CS 32
#define VS1053_DCS 33
#define VS1053_DREQ 35

#define VOLUME 100 // volume level 0-100

uint8_t bytesread = 0;
uint8_t mp3buff[32]; // vs1053 likes 32 bytes at a time

stations play_station;

VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);

bool status = true;

sdHelper objSdHelper;
wifiHelper objWifiHelper;

void initMP3Decoder()
{
  player.begin();
  player.switchToMp3Mode(); // optional, some boards require this
  player.setVolume(VOLUME);
}

void displayText(String text) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F(text.c_str()));
  display.display();      // Show initial text
  delay(100);
}

bool selectStation(uint8_t value)
{
  bool status = false;
  status = objSdHelper.getStation(initial_station, play_station);

  return status;
}

void setup()
{

  Serial.begin(9600);
  delay(500);
  Serial.println("--------------------------");
  Serial.println("esp32InternetRadio started");
  Serial.println("--------------------------");

  // initialize the pushbutton pin as an input:
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);

  SPI.begin();

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000); // Pause for 1 seconds

  displayText("Reading SD");

  // ----- Get the SSID and PSW -----
  Serial.println("Reading SD card");
  status = objSdHelper.initSD();
  if (!status)
  {
    Serial.println("SD not mounted");
  }

  String ssid, pwd;
  if (status)
  {
    status = objSdHelper.readCred(ssid, pwd);
  }
  if (!status)
  {
    Serial.println("Read failed");
  }
  // --------------------------------

  displayText("Reading station");
  // ------- Read station list ------
  total_aval_stations = objSdHelper.readStationList();
  Serial.printf("Available stations: %d\n", total_aval_stations);

  if (status && (total_aval_stations > 0))
  {
    status = selectStation(initial_station);
  }

  if (!status)
  {
    Serial.println("Requested station not found");
  }
  else
  {
    Serial.println(play_station.name.c_str());
  }

  objSdHelper.closeSD();
  // --------------------------------

  initMP3Decoder();

  displayText("Connect WiFi");
  // ----- Connecting to WiFi -------
  Serial.println("Connecting to WiFi");

  Serial.println(ssid);

  if (status)
  {
    status = objWifiHelper.connectWifi(ssid, pwd, 5);
  }
  if (status)
  {
    Serial.println("WiFi connected");
  }
  else
  {
    Serial.println("WiFi not connected");
  }
  // --------------------------------

  displayText("Connect Client");
  // ----- Connecting to Station/Client -------
  if (status)
  {
    status = objWifiHelper.connectClient(&play_station);
  }
  if (status)
  {
    Serial.println("Client connected");
  }
  else
  {
    Serial.println("Client not connected");
  }
  // --------------------------------

  displayText("Streaming " + play_station.name);
}

void loop()
{
  if (status)
  {
    // read the state of the pushbutton value
    buttonUpStateOld = buttonUpState;
    buttonDownStateOld = buttonDownState;
    buttonUpState = digitalRead(buttonUp);
    buttonDownState = digitalRead(buttonDown);

    if((buttonUpStateOld == HIGH) && (buttonUpState == LOW))
    {
      //Station Up
      initial_station++;
      if(initial_station > (total_aval_stations - 1))
      {
        initial_station = 0;
      }

      bool status_ = selectStation(initial_station);
      if (!status_)
      {
        displayText("Station not found");
      }
      else
      {
        displayText("Streaming " + play_station.name);
        status_ = objWifiHelper.connectClient(&play_station);
      }
    } else if((buttonDownStateOld == HIGH) && (buttonDownState == LOW))
    {
      //Station down
      initial_station--;
      if(initial_station < 0)
      {
        initial_station = (total_aval_stations - 1);
      }

      bool status_ = selectStation(initial_station);
      if (!status_)
      {
        displayText("Station not found");
      }
      else
      {
        displayText("Streaming " + play_station.name);
        status_ = objWifiHelper.connectClient(&play_station);
      }
    }

    bytesread = objWifiHelper.readStream(mp3buff, &play_station);
    player.playChunk(mp3buff, bytesread);
  }
}
