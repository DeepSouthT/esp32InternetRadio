/*******************************
 * sdhelper.h
 *
 * Created: 28.09.2020 20:30:00
 * Author : DeepSouthT
 * 
 * Description:
 *     ToDo
 *
 * Last modified: 23.11.2020
 *******************************/

#ifndef __SDHELPER_H__
#define __SDHELPER_H__

#include <list>
#include "globalConfig.h"

class sdHelper
{
public:
  bool initSD(void);
  void closeSD(void);
  bool readCred(String &ssid, String &psw);
  uint8_t readStationList(void);
  bool getStation(uint8_t station_number, stations &station);
  uint8_t getTotalAvalStations(void) { return total_aval_stations; };

private:
  uint8_t total_aval_stations = 0;
  const char *cred_path = "/cred.txt";
  const char *stat_path = "/station.txt";
  void prepareString(char start_char, char replacer_char, char *data, unsigned int data_len);

  std::list<stations> list_of_stations;
};

#endif //__SDHELPER_H__