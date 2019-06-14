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

public:
    Weatherstation(BMP280 &BMP280, oled &display);
    void startUp();
};

#endif //KROMWORKSWEATHERSTATION_HPP
