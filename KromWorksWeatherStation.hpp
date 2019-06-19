#ifndef KROMWORKSWEATHERSTATION_HPP
#define KROMWORKSWEATHERSTATION_HPP
#include "hwlib.hpp"
#include "BMP280.hpp"
#include "WeatherStationDisplay.hpp"

class Weatherstation
{
private:
    BMP280 &chip;
    WeatherStationDisplay &display;
    hwlib::pin_in &button;
    bool firstMeasurement = 1;

    //struct to keep track of all the measurements, (untill maxMeas) to show in a graphic.
    struct WeatherstationData
    {
        uint8_t maxnMeas = 30;
        uint8_t n = 0;
        int8_t tempArray[30] = {};
        //test array voor de grafiek
        //int8_t tempArray[30] = {5, 10, 20, 24, -10, 5, 10, 20, 24, -10, 5, 10, 20, 24, -10, 5, 10, 20, 24, -10, 5, 10, 20, 24, -10, 5, 10, 20, 24, -10};
        uint16_t pressArray[30] = {};
        void pushBack(int8_t dataTemp, uint16_t dataPress);
        void popBack();
        void wipeData();
    };

    WeatherstationData data;
    void measurementCyle();

public:
    Weatherstation(BMP280 &BMP280, WeatherStationDisplay &display, hwlib::pin_in &button);
    void startUp();
    void measurementWithInterval(uint8_t timeInMinutes);
    void drawChart();
};

#endif //KROMWORKSWEATHERSTATION_HPP
