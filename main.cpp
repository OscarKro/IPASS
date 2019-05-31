#include "hwlib.hpp"
#include "oled.hpp"

int main(void)
{
    oled oleddisplay(hwlib::target::pins::scl, hwlib::target::pins::sda);
    int x = 5;
    oleddisplay.drawTemp(x);
}
