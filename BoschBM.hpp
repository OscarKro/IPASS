#ifndef BOSCHBM_HPP
#define BOSCHBM_HPP
#include "hwlib.hpp"
//this class abstracts the chip needed for a weatherstation (or something else) using i2c. It can control a BMP180 or BMP280
class BoschBM
{
private:
    hwlib::i2c_bus &bus;

public:
    BoschBM(hwlib::i2c_bus &bus);
    virtual void selectRegister(const uint8_t slaveAdress, const uint8_t registerAdress);
    virtual uint8_t readSingleByte(const uint8_t slaveAdress, const uint8_t registerAdress);
    virtual void writeSingleByte(const uint8_t slaveAdress, const uint8_t registerAdress, const uint8_t byte);
    virtual void burstRead(const uint8_t slaveAdress, const uint8_t startRegister, uint8_t * buffer, const uint8_t n);
};
#endif //BOSCHBM_HPP