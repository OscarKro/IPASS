#ifndef KROMWORKSWEATHERSTATION_HPP
#define KROMWORKSWEATHERSTATION_HPP
#include "hwlib.hpp"
#include "EnvironmentReader.hpp"
#include "WeatherStationDisplay.hpp"

class Weatherstation
{
private:
    EnvironmentReader &chip;
    WeatherStationDisplay &display;
    hwlib::pin_in &button;
    bool firstMeasurement = 1;

    //struct to keep track of all the measurements, (untill maxMeas) to show in a graphic.
    struct WeatherstationData
    {
        uint8_t maxnMeas = 30;
        uint8_t n = 0;
        int16_t tempArray[30] = {};
        //test array voor de grafiek
        // int16_t tempArray[30] = {
        //     -2000,
        //     -2000,
        //     -2000,
        //     -3100,
        //     -3100,
        //     -3100,
        //     -2000,
        //     -2000,
        //     -2000,
        //     -1000,
        //     -1000,
        //     -1000,
        //     0,
        //     0,
        //     0,
        //     500,
        //     500,
        //     500,
        //     1000,
        //     1000,
        //     1000,
        //     2000,
        //     2000,
        //     2000,
        //     3100,
        //     3100,
        //     3100,
        //     2000,
        //     2000,
        //     2000
        // };
        uint32_t pressArray[30] = {};
        void pushBack(int16_t dataTemp, uint32_t dataPress);
        void popBack();
        void wipeData();
    };

    WeatherstationData data;

public:
    Weatherstation(EnvironmentReader &BMP280, WeatherStationDisplay &display, hwlib::pin_in &button);
    void startUp();
    void drawTempAndPress();
    void measurementWithInterval(uint8_t timeInMinutes);
    void drawChart();
};

#endif //KROMWORKSWEATHERSTATION_HPP
