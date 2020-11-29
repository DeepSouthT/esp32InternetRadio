/*******************************
 * sdhelper.cpp
 *
 * Created: 28.09.2020 20:30:00
 * Author : DeepSouthT
 * 
 * Description:
 *     ToDo
 *
 * Last modified: 23.10.2020
 *******************************/

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "sdhelper.h"

#define SSID_POS 0        // Position of SSID in the credential text file
#define SSID_LEN 20       // Maximum length of SSID
#define PWD_POS 1         // Position of pasword in the credential text file
#define PWD_LEN 30        // Maximum length of pasword
#define CRED_FILE_LEN 2   // Maximum number of line expected in the credential text file
#define STAT_FILE_LEN 250 // Maximum number of line expected in the station text file
#define HOST_LEN 75
#define PATH_LEN 75
#define NAME_LEN 25
#define PORT_LEN 15

bool sdHelper::initSD(void)
{
    // Get the SD card object
    if (!SD.begin())
    {
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

void sdHelper::prepareString(char start_char, char replacer_char, char *data, unsigned int data_len)
{
    bool valid_string_ended = false;
    for (unsigned int i = 0; i < data_len; i++)
    {
        if (data[i] == start_char)
        {
            valid_string_ended = true;
        }

        if (valid_string_ended == true)
        {
            data[i] = replacer_char;
        }
    }
}

bool sdHelper::readCred(String &ssid, String &psw)
{
    File file = SD.open(cred_path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return false;
    }

    char char_tmp = ' ';
    unsigned int line_counter = 0;
    char ssid_tmp[SSID_LEN] = {0};
    unsigned int ssid_counter = 0;
    char pwd_tmp[PWD_LEN] = {0};
    unsigned int pwd_counter = 0;

    // Read the first two lines for the credentials
    while ((file.available()) && (line_counter < CRED_FILE_LEN))
    {
        (void)file.readBytes(&char_tmp, 1);

        if (line_counter == SSID_POS)
        {
            ssid_tmp[ssid_counter] = char_tmp;
            ssid_counter++;
        }
        else if (line_counter == PWD_POS)
        {
            pwd_tmp[pwd_counter] = char_tmp;
            pwd_counter++;
        }

        while ((char_tmp != '\n') && file.available())
        {
            (void)file.readBytes(&char_tmp, 1);

            if (line_counter == SSID_POS)
            {
                ssid_tmp[ssid_counter] = char_tmp;
                ssid_counter++;
            }
            else if (line_counter == PWD_POS)
            {
                pwd_tmp[pwd_counter] = char_tmp;
                pwd_counter++;
            }
        }

        line_counter++;
    }

    // Expecting two lines in the credential text file
    if (line_counter < 1)
    {
        file.close();
        return false;
    }

    prepareString('\r', '\0', ssid_tmp, SSID_LEN);
    prepareString('\r', '\0', pwd_tmp, PWD_LEN);

    ssid = ssid_tmp;
    psw = pwd_tmp;

    file.close();

    return true;
}

uint8_t sdHelper::readStationList(void)
{
    const char name_key[6] = "name=";
    const char host_key[6] = "host=";
    const char path_key[6] = "path=";
    const char port_key[6] = "port=";
    const uint8_t key_len = 5;
    total_aval_stations = 0;

    char line[HOST_LEN] = {0};
    bool station_complete = false;
    unsigned int line_counter = 0;
    std::string line_str, name_str, host_str, path_str, port_str;
    stations station;

    File file = SD.open(stat_path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return false;
    }

    while ((file.available()) && (line_counter < STAT_FILE_LEN))
    {
        file.readBytesUntil('\n', line, HOST_LEN);

        line_str = line;
        memset(line, 0, HOST_LEN);

        if (line_str.find(name_key) != std::string::npos)
        {
            name_str = line_str.erase(0, key_len);
            station.name = name_str.c_str();
            station_complete = false;
        }
        else if (line_str.find(host_key) != std::string::npos)
        {
            host_str = line_str.erase(0, key_len);
            station.host = host_str.c_str();
            station_complete = false;
        }
        else if (line_str.find(path_key) != std::string::npos)
        {
            path_str = line_str.erase(0, key_len);
            station.path = path_str.c_str();
            station_complete = false;
        }
        else if (line_str.find(port_key) != std::string::npos)
        {
            port_str = line_str.erase(0, key_len);
            station.port = atoi(port_str.c_str());
            station_complete = true;
        }
        else
        {
            station_complete = false;
        }

        if (station_complete)
        {
            list_of_stations.push_back(station);
        }

        line_counter++;
    }

    file.close();
    total_aval_stations = list_of_stations.size();

    return total_aval_stations;
}

bool sdHelper::getStation(uint8_t station_number, stations &station)
{
    if (station_number < 0 || station_number > total_aval_stations)
    {
        return false;
    }

    std::list<stations>::iterator it = list_of_stations.begin();
    std::advance(it, station_number);
    station = *it;

    return true;
}