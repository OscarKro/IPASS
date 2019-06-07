#include "hwlib.hpp"
#include "oled.hpp"
#include "BMP280.hpp"

int main(void)
{
    int counter = 0;
    int32_t *measurement = 0;
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto mainBus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
    oled oleddisplay(mainBus);
    hwlib::wait_ms(10);
    BMP280 sensor(mainBus);
    sensor.setMode();
    sensor.readId();
    while (true)
    {
        sensor.setMode();
        counter++;
        measurement = sensor.readPTRegisters();
        hwlib::cout << "measurement: " << hwlib::dec << counter << hwlib::endl;
        hwlib::cout << "pressure dec: " << hwlib::dec << measurement[0] << hwlib::endl;
        hwlib::cout << "temperature dec: " << hwlib::dec << measurement[1] << hwlib::endl;
        hwlib::cout << "pressure bin: " << hwlib::bin << measurement[0] << hwlib::endl;
        hwlib::cout << "temperature bin: " << hwlib::bin << measurement[1] << hwlib::endl;
        oleddisplay.drawTemp(measurement[0]);
        sensor.reset();
        hwlib::wait_ms(10000);
    }
}
