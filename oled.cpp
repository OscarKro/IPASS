#include "oled.hpp"
#include <string>

oled::oled(hwlib::i2c_bus_bit_banged_scl_sda &bus)
    : bus(bus), display(bus, 0x3c), window(display, font) {}

void oled::resetCursor()
{
    window.cursor_set(hwlib::xy(0,1));
    display.flush();
}
void oled::clearScreen()
{
    display.clear();
    display.flush();
}
void oled::drawInt32T(const int32_t x)
{
    window << hwlib::dec << x;
    display.flush();
}
void oled::drawUint32T(const uint32_t x)
{
    window << hwlib::dec << x;
    display.flush();
}
void oled::drawByte(const uint8_t x)
{
    window << hwlib::dec << x;
    display.flush();
}
void oled::drawText(const char *s)
{
    window << s;
    display.flush();
}