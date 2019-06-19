#include "WeatherStationDisplay.hpp"


WeatherStationDisplay::WeatherStationDisplay(hwlib::window &w, hwlib::terminal &terminal)
    : w(w), terminal(terminal) {}

//function to reset the cursur back to the (almost) top left
void WeatherStationDisplay::resetCursor(uint8_t x, uint8_t y)
{
    terminal.cursor_set(hwlib::xy(x, y));
}

//function to clear the screen
void WeatherStationDisplay::clearScreen()
{
    terminal.clear();
}
//function to draw a int8_t to the screen (for the temperature)
void WeatherStationDisplay::drawInt(const int8_t x)
{
    terminal << hwlib::dec << x;
}

//function to draw an unsigned int8_t to the screen (for the pressure in hPa)
void WeatherStationDisplay::drawInt(const uint16_t x)
{
    terminal << hwlib::dec << x;
}

//function to draw a single byte in dec to the screen
void WeatherStationDisplay::drawByte(const uint8_t x)
{
    terminal << hwlib::dec << x;
}

//function to draw a complete string to the screen
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