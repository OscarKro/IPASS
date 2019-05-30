#include "bmpoled.hpp"

bmpoled::bmpoled(hwlib::target::pin_oc sclOled, hwlib::target::pin_oc sdaOled) 
: sclOled(sclOled), sdaOled(sdaOled), busOled(sclOled,sdaOled), display(busOled, 0x3c) {}

void bmpoled::drawCircles()
{
    hwlib::graphics_random_circles(display, 5);
}