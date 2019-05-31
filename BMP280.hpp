#ifndef BMP280_HPP
#define BMP280_HPP
#include "hwlib.hpp"
//bmp slave adress = 0b1110110 (0x76) 0 voor write, 1 voor read aan het einde.
class BMP280
{
private:
hwlib::i2c_bus_bit_banged_scl_sda bus;
int adress = 0b1110110;

public:
BMP280(hwlib::pin_oc& scl, hwlib::pin_oc& sda);
void select();
};
#endif //BMP280_HPP