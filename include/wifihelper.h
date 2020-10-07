/*******************************
 * wifihelper.h
 *
 * Created: 13.09.2020 20:00:00
 * Author : DeepSouthT
 * 
 * Description:
 *     ToDo
 *
 * Last modified: 13.09.2020
 *******************************/

#ifndef __WIFIHELPER_H__
#define __WIFIHELPER_H__

class wifiHelper
{
public:
  bool connectWifi(String ssid, String password, unsigned int repetition);
  void disconnectWifi(void);
};

#endif //__WIFIHELPER_H__