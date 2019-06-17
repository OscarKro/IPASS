#include "hwlib.hpp"
#include "oled.hpp"
#include "BMP280.hpp"
#include "KromWorksWeatherStation.hpp"

int main(void)
{
    auto button = hwlib::target::pin_in(hwlib::target::pins::d2);
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto mainBus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
    oled oleddisplay(mainBus);
    hwlib::wait_ms(10);
    BMP280 sensor(mainBus);
    hwlib::wait_ms(10);
    Weatherstation station(sensor, oleddisplay, button);
    station.startUp();
    while (true)
    {
        station.measurementWithInterval(1);
    }
}
