#ifndef BMP280_HPP
#define BMP280_HPP
#include "hwlib.hpp"
class BMP280
{
private:
    hwlib::i2c_bus &bus;

    enum adresses : const uint8_t
    {
        resetAdress = 0xE0,
        idAdress = 0xD0,
        statusAdress = 0xF3,
        ctrl_measAdress, //0xF4
        configAdress, //0xF5
        i2cAdress, //0XF6
        pressureAdress1, //0xF7
        pressureAdress2, //0XF8
        pressureAdress3, //0xF9
        tempAdress1, //0xFA
        tempAdress2, //0xFB
        tempAdress3 //0xFC
    };

public:
    BMP280(hwlib::i2c_bus &bus);
    void selectRegister(const uint8_t adress);
    uint8_t readSingleByte(const uint8_t adress);
    void writeSingleByte(const uint8_t adress, const uint8_t byte);
    void readPTRegisters(int32_t &totalPress, int32_t &totalTemp);
    uint8_t readId();
    void setMode();
    void reset();
};
#endif //BMP280_HPP
