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
    while (true)
    {
        sensor.readPTRegisters(global::press, global::temp);
        hwlib::cout << "pressure dec: " << hwlib::dec << global::press << hwlib::endl;
        hwlib::cout << "temperature dec: " << hwlib::dec << global::temp << hwlib::endl;
        hwlib::cout << "pressure bin: " << hwlib::showbase << hwlib::bin << global::press << hwlib::endl;
        hwlib::cout << "temperature bin: " << hwlib::showbase << hwlib::bin << global::temp << hwlib::endl;
        oleddisplay.drawTemp(global::temp);
        hwlib::wait_ms(60000);
    }
}
