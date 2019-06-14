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
    void resetCursor();
    void clearScreen();
    void drawTemp(const int32_t x);
    void drawText(const char *s);
};

#endif //OLED_HPP
