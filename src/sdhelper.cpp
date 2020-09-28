/*******************************
 * sdhelper.cpp
 *
 * Created: 28.09.2020 20:30:00
 * Author : DeepSouthT
 * 
 * Description:
 *     ToDo
 *
 * Last modified: 28.09.2020
 *******************************/

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "sdhelper.h"

bool sdHelper::initSD(void)
{
    // Get the SD card object
    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return false;
    }

    return true;
}

void sdHelper::closeSD(void)
{
    // Close the SD card object
    Serial.println("Closing SD card");
    SD.end();
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}