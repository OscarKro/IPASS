#include "hwlib.hpp"
#include "oled.hpp"
#include "BMP280.hpp"
#include "KromWorksWeatherStation.hpp"

int main(void)
{
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto mainBus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
    oled oleddisplay(mainBus);
    hwlib::wait_ms(10);
    BMP280 sensor(mainBus);
    hwlib::wait_ms(10);
    Weatherstation station(sensor, oleddisplay);
    station.startUp();
    hwlib::cout << "p1 " << sensor.returnData().dig_p1 << hwlib::endl;
    hwlib::cout << "p2 " << sensor.returnData().dig_p2 << hwlib::endl;
    hwlib::cout << "p3 " << sensor.returnData().dig_p3 << hwlib::endl;
    hwlib::cout << "p4 " << sensor.returnData().dig_p4 << hwlib::endl;
    hwlib::cout << "p5 " << sensor.returnData().dig_p5 << hwlib::endl;
    hwlib::cout << "p6 " << sensor.returnData().dig_p6 << hwlib::endl;
    hwlib::cout << "p7 " << sensor.returnData().dig_p7 << hwlib::endl;
    hwlib::cout << "p8 " << sensor.returnData().dig_p8 << hwlib::endl;
    hwlib::cout << "p9 " << sensor.returnData().dig_p9 << hwlib::endl;
    hwlib::cout << "t1 " << sensor.returnData().dig_t1 << hwlib::endl;
    hwlib::cout << "t1 " << sensor.returnData().dig_t2 << hwlib::endl;
    hwlib::cout << "t1 " << sensor.returnData().dig_t3 << hwlib::endl;
    hwlib::cout << "--------------------------------------------" << hwlib::endl;
    while (true)
    {
        station.intervalMeasurement(60000);
    }
}
