#include "bmpoled.hpp"

bmpoled::bmpoled(hwlib::target::pin_oc sclOled, hwlib::target::pin_oc sdaOled)
    : sclOled(sclOled), sdaOled(sdaOled), busOled(sclOled, sdaOled), display(busOled, 0x3c), window(display, font) {}

void bmpoled::drawCircles(uint8_t x)
{
    hwlib::graphics_random_circles(display, x);
}
void bmpoled::drawText(const int x)
{
    window << x << " C";
    display.flush();
}