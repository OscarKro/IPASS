#ifndef BMP280_HPP
#define BMP280_HPP
#include "hwlib.hpp"
//bmp slave adress = 0b1110110 (0x76) 0 voor write, 1 voor read aan het einde.
class BMP280
{
private:
hwlib::i2c_bus_bit_banged_scl_sda bus;

const uint8_t generalAdress = 0x76;
const uint8_t statusAdress = 0xF3;
const uint8_t ctrlMeasAdress = 0xF4;
const uint8_t configAdress = 0xF5;
const uint8_t pressureAdress1 = 0xF7;
const uint8_t pressureAdress2 = 0xF8;
const uint8_t pressureAdress3 = 0xF9;
const uint8_t tempAdress1 = 0xFA;
const uint8_t tempAdress2 = 0xFB;
const uint8_t tempAdress3 = 0xFC;

public:
BMP280(hwlib::i2c_bus_bit_banged_scl_sda bus);
void start();
};
#endif //BMP280_HPP