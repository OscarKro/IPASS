#ifndef BMP280_HPP
#define BMP280_HPP
#include "hwlib.hpp"
//bmp slave adress = 0b1110110 (0x76) 0 voor write, 1 voor read aan het einde.
class BMP280
{
private:
hwlib::i2c_bus_bit_banged_scl_sda bus;
const uint8_t adress = 0x76;

public:
BMP280(hwlib::i2c_bus_bit_banged_scl_sda bus);
void start();
};
#endif //BMP280_HPP