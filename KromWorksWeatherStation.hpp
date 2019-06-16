#ifndef KROMWORKSWEATHERSTATION_HPP
#define KROMWORKSWEATHERSTATION_HPP
#include "hwlib.hpp"
#include "BMP280.hpp"
#include "oled.hpp"

class Weatherstation
{
private:
    BMP280 &chip;
    oled &display;
    bool firstMeasurement = true;

    //struct to keep track of all the measurements, (untill maxMeas) to show in a graphic
    struct WeatherstationData
    {
        uint8_t maxMeas = 10;
        uint8_t count = 0;
        int8_t tempArray[60] = {};
        uint16_t pressArray[60] = {};
        void pushBack(int8_t data)
        {
            if (count < maxMeas)
            {
                tempArray[count] = data;
                count++;
            }
            else if (count >= maxMeas)
            {
                count = 0;
                tempArray[count] = data;
            }
        }
        void pushBack(uint16_t data)
        {
            if (count < maxMeas)
            {
                pressArray[count] = data;
                count++;
            }
            else if (count >= maxMeas)
            {
                count = 0;
                pressArray[count] = data;
            }
        }
        void popBack(char arraytype)
        {
            if (arraytype == 't')
            {
                tempArray[count] = 0;
                count--;
            }
            else if (arraytype == 'p')
            {
                pressArray[count] = 0;
                count--;
            }
        }
        void wipeData()
        {
            for (uint8_t i = 0; i < count; i++)
            {
                tempArray[i] = 0;
                pressArray[i] = 0;
            }
        }
    };

    WeatherstationData data;

public:
    Weatherstation(BMP280 &BMP280, oled &display);
    void startUp();
    void measurementCyle();
    void intervalMeasurement(uint16_t time);
};

#endif //KROMWORKSWEATHERSTATION_HPP
