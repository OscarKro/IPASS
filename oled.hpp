#ifndef OLED_HPP
#define OLED_HPP
#include "hwlib.hpp"
class oled
{
private:
    hwlib::i2c_bus_bit_banged_scl_sda &bus;
    hwlib::glcd_oled display;

    hwlib::font_default_16x16 font;
    hwlib::terminal_from window;

public:
    oled(hwlib::i2c_bus_bit_banged_scl_sda &bus);
    void drawTemp(const uint32_t x);
};

#endif //OLED_HPP
