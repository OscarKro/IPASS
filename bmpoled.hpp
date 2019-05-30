#ifndef BMPOLED_HPP
#define BMPOLED_HPP
#include "hwlib.hpp"
class bmpoled
{
private:
    hwlib::target::pin_oc sclOled;
    hwlib::target::pin_oc sdaOled;
    hwlib::i2c_bus_bit_banged_scl_sda bus;
    hwlib::glcd_oled display;

    hwlib::font_default_16x16 font;
    hwlib::terminal_from window;

    const uint8_t bmpAdress = 0b1110110;
    

public:
    bmpoled(hwlib::target::pin_oc sclOled, hwlib::target::pin_oc sdaOled);
    void drawTemp(const int x);
    void readBMP();
    void writeBMP();
};


#endif //BMPOLED_HPP
