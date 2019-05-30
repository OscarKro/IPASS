#include "hwlib.hpp"
#include "bmpoled.hpp"

int main(void)
{
    bmpoled oleddisplay(hwlib::target::pins::scl, hwlib::target::pins::sda);
    //oleddisplay.drawCircles(8);
    int x = 5;
    oleddisplay.drawText(x);
}
