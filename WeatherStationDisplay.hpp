#ifndef WEATHERSTATIONDISPLAY_HPP
#define WEATHERSTATIONDISPLAY_HPP
#include "hwlib.hpp"
class WeatherStationDisplay
{
private:
    hwlib::window &w;
    hwlib::terminal &terminal;

public:
    WeatherStationDisplay();
    WeatherStationDisplay(hwlib::window &w, hwlib::terminal &terminal);
    void resetCursor(uint8_t x, uint8_t y);
    void clearScreen();
    void drawInt(const int8_t x);
    void drawInt(const uint8_t x);
    void drawInt(const int16_t x);
    void drawInt(const uint16_t x);
    void drawText(const char *s);
    void drawLine(const hwlib::xy &start, const hwlib::xy &end);
    void flush();
};

#endif //WEATHERSTATIONDISPLAY_HPP
