#ifndef WEATHERSTATIONDISPLAY_HPP
#define WEATHERSTATIONDISPLAY_HPP
/// @file
#include "hwlib.hpp"
/// \brief
/// Weatherstation Display class
/// \details
/// This class has functions needed to use the display from a KromWorks WeatherStation. It includes the Hardware library from Wouter van Ooijen called "hwlib.hpp". All the 
/// hardware communication is done in that library and all rights for hwlib are reserved for Wouter van Ooijen.
class WeatherStationDisplay
{
private:
    hwlib::window &w;
    hwlib::terminal &terminal;

public:
    /// \brief
    /// Constructor
    /// \details
    /// This constructor requires a hwlib window and a hwlib terminal. See the documentation from hwlib for more details.
    WeatherStationDisplay(hwlib::window &w, hwlib::terminal &terminal);

    /// \brief
    /// Function to reset the cursor
    /// \details
    /// This member function resets the cursor on the display to x and y given in the parameters. It uses functions from hwlib and an xy object from
    /// hwlib.
    void resetCursor(uint8_t x, uint8_t y);

    /// \brief
    /// Function to clear the screen
    /// \details
    /// This member function clears the display using functions from hwlib.
    void clearScreen();

    /// \brief
    /// Function to draw an integer
    /// \details
    /// This member function draws a signed 8 bits integer to the display. The integer to be drawed needs to be given in its parameters.
    /// It uses operators from hwlib.
    void drawInt(const int8_t x);

    /// \brief
    /// Function to draw an integer
    /// \details
    /// This member function draws an unsigned 8 bits integer to the display. The integer to be drawed needs to be given in its parameters.
    /// It uses operators from hwlib.
    void drawInt(const uint8_t x);

    /// \brief
    /// Function to draw an integer
    /// \details
    /// This member function draws a signed 16 bits integer to the display. The integer to be drawed needs to be given in its parameters.
    /// It uses operators from hwlib.
    void drawInt(const int16_t x);

    /// \brief
    /// Function to draw an integer
    /// \details
    /// This member function draws an unsigned 16 bits integer to the display. The integer to be drawed needs to be given in its parameters.
    /// It uses operators from hwlib.
    void drawInt(const uint16_t x);

    /// \brief
    /// Function to draw a string
    /// \details
    /// This member function draws a string to the display. If the string is to large for the display, you will have to go to the next line
    /// manually. The string needs to be given in its parameters. It uses operators from hwlib.
    void drawText(const char *s);

    /// \brief
    /// Function to draw a line
    /// \details
    /// This member function draws a line to the display. It requires two xy object from hwlib as parameters. They are the start xy position and the 
    /// end xy position on the display. It uses functions from hwlib. 
    void drawLine(const hwlib::xy &start, const hwlib::xy &end);

    /// \brief
    /// Function to flush
    /// \details
    /// This member function flushes the data to the actual screen. Whenever you want something written or drawn to the screen, you will have to flush
    /// at the end. It uses functions from hwlib.
    void flush();
};

#endif //WEATHERSTATIONDISPLAY_HPP
