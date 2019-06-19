#include "hwlib.hpp"
#include "WeatherStationDisplay.hpp"
#include "BMP280.hpp"
#include "KromWorksWeatherStation.hpp"

int main(void)
{
    auto button = hwlib::target::pin_in(hwlib::target::pins::d2);
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto font = hwlib::font_default_16x16();
    auto mainBus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);

    auto oled = hwlib::glcd_oled(mainBus);
    auto terminal = hwlib::terminal_from(oled, font);

    auto oledDisplay = WeatherStationDisplay(oled, terminal);
    BMP280 sensor(mainBus);
    Weatherstation station(sensor, oledDisplay, button);
    station.startUp();
    while (true)
    {
        station.measurementWithInterval(1);
    }
}
