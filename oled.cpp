#include "oled.hpp"

oled::oled(hwlib::i2c_bus_bit_banged_scl_sda &bus)
    : bus(bus), display(bus, 0x3c), window(display, font) {}

void oled::resetCursor()
{
    //function to reset the cursur back to the (almost) top left
    window.cursor_set(hwlib::xy(0, 1));
    display.flush();
}
void oled::clearScreen()
{
    //function to clear the screen
    display.clear();
    display.flush();
}
void oled::drawInt32T(const int32_t x)
{
    //function to draw a signed int 32 to the screen (for the temperature)
    window << hwlib::dec << x;
    display.flush();
}
void oled::drawUint32T(const uint32_t x)
{
    //function to draw an unsigned int 32 to the screen (for the pressure in hPa)
    window << hwlib::dec << x;
    display.flush();
}
void oled::drawByte(const uint8_t x)
{
    //function to draw a single byte in dec to the screen
    window << hwlib::dec << x;
    display.flush();
}
void oled::drawText(const char *s)
{
    //function to draw a complete string to the screen
    window << s;
    display.flush();
}