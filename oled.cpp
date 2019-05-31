#include "oled.hpp"

oled::oled(hwlib::i2c_bus_bit_banged_scl_sda bus)
    : bus(bus) ,display(bus, 0x3c), window(display, font) {}
void oled::drawTemp(const int x)
{
    window << x << "\n" << "Celsius";
    display.flush();
}