#include "hwlib.hpp"
#include "oled.hpp"
#include "BMP280.hpp"
#include "KromWorksWeatherStation.hpp"

int main(void)
{
    uint8_t n = 30;
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto mainBus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
    oled oleddisplay(mainBus);
    hwlib::wait_ms(10);
    BMP280 sensor(mainBus);
    hwlib::wait_ms(10);
    Weatherstation station(sensor, oleddisplay);
    station.startUp();
    while (true)
    {
        n++;
        if (n == 30)
        {
            station.drawChart();
            break;
        }
        station.measurementWithInterval(60000);
    }
}
