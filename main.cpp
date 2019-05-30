#include "hwlib.hpp"
#include "bmpoled.hpp"

int main(void)
{
    bmpoled oleddisplay(hwlib::target::pins::scl, hwlib::target::pins::sda);
    int x = 5;
    oleddisplay.drawTemp(x);
}
