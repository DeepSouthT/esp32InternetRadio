/*******************************
 * globalConfig.h
 *
 * Created: 23.11.2020 21:36:00
 * Author : DeepSouthT
 * 
 * Description:
 *     ToDo
 *
 * Last modified: 29.11.2020
 *******************************/

#ifndef __GLOBALCONFIG_H__
#define __GLOBALCONFIG_H__

struct stations
{
  String name;
  String host;
  String path;
  unsigned int port = 0;
  bool availability = false;
};

#endif //__GLOBALCONFIG_H__