#include "hwlib.hpp"
#include "oled.hpp"
#include "BMP280.hpp"

namespace global
{
    int32_t press;
    int32_t temp;
}
int main(void)
{
    
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto mainBus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
    oled oleddisplay(mainBus);
    hwlib::wait_ms(10);
    BMP280 sensor(mainBus);
    hwlib::wait_ms(10);
    oleddisplay.drawReset();
    sensor.reset();
    sensor.readTempParam();
    sensor.readPressParam();
    hwlib::cout << sensor.returnData().dig_t1 << hwlib::endl;
    hwlib::cout << sensor.returnData().dig_t2 << hwlib::endl;
    hwlib::cout << sensor.returnData().dig_t3 << hwlib::endl;
    hwlib::cout << sensor.returnData().dig_p1 << hwlib::endl;
    hwlib::cout << sensor.returnData().dig_p2 << hwlib::endl;
    hwlib::cout << sensor.returnData().dig_p3 << hwlib::endl;
    hwlib::cout << sensor.returnData().dig_p4 << hwlib::endl;
    hwlib::cout << sensor.returnData().dig_p5 << hwlib::endl;
    hwlib::cout << sensor.returnData().dig_p6 << hwlib::endl;
    hwlib::cout << sensor.returnData().dig_p7 << hwlib::endl;
    hwlib::cout << sensor.returnData().dig_p8 << hwlib::endl;
    hwlib::cout << sensor.returnData().dig_p9 << hwlib::endl;
    hwlib::cout << "einde --------------------------" << hwlib::endl;
}
