#ifndef BMP280_HPP
#define BMP280_HPP
#include "hwlib.hpp"
class BMP280
{
private:
hwlib::i2c_bus& bus;

const uint8_t i2cAdress = 0x76;//0b1110110
const uint8_t statusAdress = 0xF3;
const uint8_t modeAdress = 0xF4;//00 = sleep mode, 01 en 10 is forced mode 11 is normal mode;
const uint8_t configAdress = 0xF5;
const uint8_t pressureAdress1 = 0xF7;
const uint8_t pressureAdress2 = 0xF8;
const uint8_t pressureAdress3 = 0xF9;
const uint8_t tempAdress1 = 0xFA;
const uint8_t tempAdress2 = 0xFB;
const uint8_t tempAdress3 = 0xFC;
const uint8_t resetAdress = 0xE0;
const uint8_t idAdress = 0xD0;

public:
BMP280(hwlib::i2c_bus& bus);
void selectRegister(const uint8_t adress);
uint8_t readTemp();
uint8_t readId();
void setMode(char ch);
};
#endif //BMP280_HPP
