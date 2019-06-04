#ifndef BMP280_HPP
#define BMP280_HPP
#include "hwlib.hpp"
class BMP280
{
private:

    hwlib::i2c_bus &bus;

    enum class adress : const uint8_t
    {
        i2cAdress = 0x76, //0b1110110
        statusAdress = 0xF3,
        modeAdress = 0xF4, //00 = sleep mode, 01 en 10 is forced mode 11 is normal mode;
        configAdress = 0xF5,
        pressureAdress1 = 0xF7,
        pressureAdress2 = 0xF8,
        pressureAdress3 = 0xF9,
        tempAdress1 = 0xFA,
        tempAdress2 = 0xFB,
        tempAdress3 = 0xFC,
        resetAdress = 0xE0,
        idAdress = 0xD0
    };

public : 
BMP280(hwlib::i2c_bus &bus);
    void selectRegister(const uint8_t adress);
    uint8_t readTemp();
    uint8_t readId();
    void setMode(char ch);
};
#endif //BMP280_HPP
