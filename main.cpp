#include "hwlib.hpp"
#include "oled.hpp"
#include "BMP280.hpp"

int main(void)
{
    oled oleddisplay(hwlib::target::pins::scl, hwlib::target::pins::sda);
    hwlib::wait_ms(5);
    BMP280 sensor(hwlib::target::pins::scl, hwlib::target::pins::sda);
    int8_t x = sensor.readTemp();
    oleddisplay.drawTemp(x);
}
