#ifndef BMP280_HPP
#define BMP280_HPP
#include "hwlib.hpp"
//bmp slave adress = 0b1110110 (0x76) 0 voor write, 1 voor read aan het einde.
class BMP280
{
private:
hwlib::i2c_bus_bit_banged_scl_sda bus;
const uint8_t adress = 0x76;
uint8_t adressTemp = 0xFB;

public:
BMP280(hwlib::pin_oc& scl, hwlib::pin_oc& sda);
int8_t readTemp();
};
#endif //BMP280_HPP