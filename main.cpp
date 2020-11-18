#include "hwlib.hpp"
#include "WeatherStationDisplay.hpp"
#include "BMP280.hpp"
#include "KromWorksWeatherStation.hpp"

int main(void)
{
    auto button = hwlib::target::pin_in(hwlib::target::pins::d5);
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::d14);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::d15);
    auto font = hwlib::font_default_16x16();
    auto mainBus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);

    auto oled = hwlib::glcd_oled(mainBus);
    auto terminal = hwlib::terminal_from(oled, font);

    auto oledDisplay = WeatherStationDisplay(oled, terminal);
    Weatherstation station(sensor, oledDisplay, button);
    hwlib::cout << sensor.readId();
    station.startUp();
    while (true)
    {
        station.measurementWithInterval(1);
    }
    //voor de presentatie
    //station.drawChart();
}
