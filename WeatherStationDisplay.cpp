#include "WeatherStationDisplay.hpp"


WeatherStationDisplay::WeatherStationDisplay(hwlib::window &w, hwlib::terminal &terminal)
    : w(w), terminal(terminal) {}


void WeatherStationDisplay::resetCursor(const uint8_t x, const uint8_t y)
{
    terminal.cursor_set(hwlib::xy(x, y));
}


void WeatherStationDisplay::clearScreen()
{
    terminal.clear();
}

void WeatherStationDisplay::drawInt(const int8_t x)
{
    terminal << hwlib::dec << x;
}


void WeatherStationDisplay::drawInt(const uint8_t x)
{
    terminal << hwlib::dec << x;
}


void WeatherStationDisplay::drawInt(const int16_t x)
{
    terminal << hwlib::dec << x;
}


void WeatherStationDisplay::drawInt(const uint16_t x)
{
    terminal << hwlib::dec << x;
}


void WeatherStationDisplay::drawText(const char *s)
{
    terminal << s;
}
void WeatherStationDisplay::drawLine(const hwlib::xy &start, const hwlib::xy &end)
{
    hwlib::line line(start, end);
    line.draw(w);
}

void WeatherStationDisplay::flush()
{
    w.flush();
}