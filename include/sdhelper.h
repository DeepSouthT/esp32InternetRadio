/*******************************
 * sdhelper.h
 *
 * Created: 28.09.2020 20:30:00
 * Author : DeepSouthT
 * 
 * Description:
 *     ToDo
 *
 * Last modified: 28.09.2020
 *******************************/

#ifndef __SDHELPER_H__
#define __SDHELPER_H__

class sdHelper
{
  public:
    bool initSD(void);
    void closeSD(void);
    bool readCred(const char* usr, const char* psw);
    void readStation(unsigned int position, const char* station);
  
  private:
    const unsigned int USR_POSITION = 0;
    const unsigned int PSW_POSITION = 1;
    const unsigned int SOS_POSITION = 2;
};

#endif //__SDHELPER_H__