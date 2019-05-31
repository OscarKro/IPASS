#ifndef OLED_HPP
#define OLED_HPP
#include "hwlib.hpp"
class oled
{
private:
    hwlib::i2c_bus_bit_banged_scl_sda bus;
    hwlib::glcd_oled display;

    hwlib::font_default_16x16 font;
    hwlib::terminal_from window;

public:
    oled(hwlib::target::pin_oc sclOled, hwlib::target::pin_oc sdaOled);
    void drawTemp(const int x);
};


#endif //OLED_HPP
