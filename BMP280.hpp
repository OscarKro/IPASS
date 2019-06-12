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
        ctrl_measAdress,
        configAdress,
        i2cAdress,
        pressureAdress1,
        pressureAdress2,
        pressureAdress3,
        tempAdress1,
        tempAdress2,
        tempAdress3,

        dig_t1Adress1 = 0x88, //unsigned short
        dig_t1Adress2, 
        dig_t2Adress1, //signed short
        dig_t2Adress2,
        dig_t3Adress1, //signed short
        dig_t3Adress2,
        dig_p1Adress1, //unsigned short
        dig_p1Adress2,
        dig_p2Adress1, //signed short
        dig_p2Adress2,
        dig_p3Adress1, //signed short
        dig_p3Adress2,
        dig_p4Adress1, //signed short
        dig_p4Adress2,
        dig_p5Adress1, //signed short
        dig_p6Adress2,
        dig_p7Adress1, //signed short
        dig_p7Adress2,
        dig_p8Adress1, //signed short
        dig_p8Adress2,
        dig_p9Adress1, //signed short
        dig_p9Adress2,
        reservedAdress1, //reserved, onleesbaar?
        reservedAdress2
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
