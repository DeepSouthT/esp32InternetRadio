/*******************************
 * wifihelper.h
 *
 * Created: 13.09.2020 20:00:00
 * Author : DeepSouthT
 * 
 * Description:
 *     ToDo
 *
 * Last modified: 29.11.2020
 *******************************/
/*
Basically a web radio stream URL consists of:
Server-Protocol (http, https)
Server-Host
Server-Port
Stream-Name (mountpoint / streampath)

and results in the following URL-structure:
http(s)://server-host:server-port/stream-name
Example: http://rs16.stream24.net:8000/horeb.mp3

If your stream server is running on HTTP standard port 80 or HTTPS standard port 443, 
the colon and port number can be omitted:
http://rs16.stream24.net/horeb.mp3
https://rs16.stream24.net/horeb.mp3

Get radio inks
https://www.radio-browser.info/
*/

#ifndef __WIFIHELPER_H__
#define __WIFIHELPER_H__

#include <list>
#include "globalConfig.h"

class wifiHelper
{
private:
  uint8_t bytesread = 0;
  bool client_status_ok_printed = false;
  bool client_status_nok_printed = false;
  unsigned int client_status_nok_counter = 0;

  bool connectClient_executed_once = false;
  char host_tst[75];
  char path_tst[75];

  // Few Radio Stations
  /*
  const char *host = "149.255.59.162";
  const char *path = "/1";
  int port = 8062;
  */

  /*
  const char *host = "realfm.live24.gr";
  const char *path = "/realfm";
  int   port = 80;
  */

  /*
  const char *host = "radiostreaming.ert.gr";
  const char *path = "/ert-kosmos";
  int   port = 80;
  */

  /*
  const char *host = "wdr-edge-1019-fra-dtag-cdn.cast.addradio.de";
  const char *path = "/wdr/wdr5/live/mp3/128/stream.mp3";
  unsigned int port = 80;
  */

  /*
  // Lost connection after ~1 Min
  const char *host = "0-24schlagervolksmusik.stream.laut.fm";
  const char *path = "/0-24_schlager_volksmusik";
  int   port = 80;
  */

public:
  bool connectWifi(String ssid, String password, unsigned int repetition);
  void disconnectWifi(void);
  bool connectClient(stations *station);
  uint8_t readStream(uint8_t *mp3buff, stations *station);
};

#endif //__WIFIHELPER_H__