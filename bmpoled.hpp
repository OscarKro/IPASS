#ifndef BMPOLED_HPP
#define BMPOLED_HPP
#include "hwlib.hpp"

class bmpoled
{
private:
    hwlib::target::pin_oc sclOled;
    hwlib::target::pin_oc sdaOled;
    hwlib::i2c_bus_bit_banged_scl_sda busOled;
    hwlib::glcd_oled display;

public:
    bmpoled(hwlib::target::pin_oc sclOled, hwlib::target::pin_oc sdaOled);
    void drawCircles();
};

#endif //BMPOLED_HPP
