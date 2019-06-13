#ifndef KROMWORKSWEATHERSTATION_HPP
#define KROMWORKSWEATHERSTATION_HPP
#include "hwlib.hpp"
#include "BMP280.hpp"
#include "oled.hpp"

class Weatherstation
{
private:
BMP280& BMP280;
oled& display;

public:
Weatherstation(BMP280& BMP280, oled& display):BMP280(BMP280), display(display) {}

};

#endif //KROMWORKSWEATHERSTATION_HPP