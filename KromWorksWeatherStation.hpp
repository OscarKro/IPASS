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
        uint8_t maxnMeas = 30;
        uint8_t n = 0;
        int8_t tempArray[30] = {};
        //test array voor de grafiek
        //int8_t tempArray[30] = {5, 10, 20, 24, -10, 5, 10, 20, 24, -10, 5, 10, 20, 24, -10, 5, 10, 20, 24, -10, 5, 10, 20, 24, -10, 5, 10, 20, 24, -10};
        uint16_t pressArray[30] = {};
        void pushBack(int8_t dataTemp, uint16_t dataPress)
        {
            if (n < maxnMeas)
            {
                tempArray[n] = dataTemp;
                pressArray[n] = dataPress;
                n++;
            }
            else if (n >= maxnMeas)
            {
                n = 0;
                tempArray[n] = dataTemp;
                pressArray[n] = dataPress;
            }
        }
        void popBack()
        {
            tempArray[n] = 0;
            pressArray[n] = 0;
            n--;
        }
        void wipeData()
        {
            for (uint8_t i = 0; i < n; i++)
            {
                tempArray[i] = 0;
                pressArray[i] = 0;
            }
        }
    };

    WeatherstationData data;
    void measurementCyle();

public:
    Weatherstation(BMP280 &BMP280, oled &display);
    void startUp();
    void measurementWithInterval(uint16_t time);
    void drawChart();
};

#endif //KROMWORKSWEATHERSTATION_HPP
