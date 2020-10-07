/*******************************
 * sdhelper.h
 *
 * Created: 28.09.2020 20:30:00
 * Author : DeepSouthT
 * 
 * Description:
 *     ToDo
 *
 * Last modified: 06.10.2020
 *******************************/

#ifndef __SDHELPER_H__
#define __SDHELPER_H__

class sdHelper
{
public:
  bool initSD(void);
  void closeSD(void);
  bool readCred(String &ssid, String &psw);
  void readStation(unsigned int position, const char *station);

private:
  const char *cred_path = "/cred.txt";
  const char *stat_path = "/stat.txt";
  void prepareString(char start_char, char replacer_char, char *data, unsigned int data_len);
};

#endif //__SDHELPER_H__