#ifndef BMPOLED_HPP
#define BMPOLED_HPP
#include "hwlib.hpp"
#include <string>
class bmpoled
{
private:
    hwlib::target::pin_oc sclOled;
    hwlib::target::pin_oc sdaOled;
    hwlib::i2c_bus_bit_banged_scl_sda busOled;
    hwlib::glcd_oled display;

    hwlib::font_default_16x16 font;
    hwlib::terminal_from window;
    

public:
    bmpoled(hwlib::target::pin_oc sclOled, hwlib::target::pin_oc sdaOled);
    void drawCircles(uint8_t x);
    void drawText(const int x);
};


#endif //BMPOLED_HPP
