#include "oled.hpp"

oled::oled(hwlib::target::pin_oc sclOled, hwlib::target::pin_oc sdaOled)
    : bus(sclOled, sdaOled) ,display(bus, 0x3c), window(display, font) {}
void oled::drawTemp(const int x)
{
    window << x << "\n" << "Celsius";
    display.flush();
}