#include "oled.hpp"

oled::oled(hwlib::i2c_bus_bit_banged_scl_sda &bus)
    : bus(bus), display(bus, 0x3c), window(display, font) {}

//function to reset the cursur back to the (almost) top left
void oled::resetCursor(uint8_t x, uint8_t y)
{
    window.cursor_set(hwlib::xy(x, y));
    display.flush();
}

//function to clear the screen
void oled::clearScreen()
{
    display.clear();
    display.flush();
}

//function to draw a int8_t to the screen (for the temperature)
void oled::drawInt(const int8_t x)
{
    window << hwlib::dec << x;
    display.flush();
}

//function to draw an unsigned int8_t to the screen (for the pressure in hPa)
void oled::drawInt(const uint16_t x)
{
    window << hwlib::dec << x;
    display.flush();
}

//function to draw a single byte in dec to the screen
void oled::drawByte(const uint8_t x)
{
    window << hwlib::dec << x;
    display.flush();
}

//function to draw a complete string to the screen
void oled::drawText(const char *s)
{
    window << s;
    display.flush();
}