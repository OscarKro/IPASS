#include "BoschBM.hpp"

BoschBM::BoschBM(hwlib::i2c_bus &bus) : bus(bus) {}

void BoschBM::selectRegister(const uint8_t slaveAdress, const uint8_t registerAdress)
{
    bus.write(slaveAdress).write(registerAdress);
}

uint8_t BoschBM::readSingleByte(const uint8_t slaveAdress, const uint8_t registerAdress)
{
    selectRegister(slaveAdress, registerAdress);
    return bus.read(slaveAdress).read_byte();
}

void BoschBM::writeSingleByte(const uint8_t slaveAdress, const uint8_t registerAdress, const uint8_t byte)
{
    auto transaction = bus.write(slaveAdress);
    transaction.write(registerAdress);
    transaction.write(byte);
}

void BoschBM::burstRead(const uint8_t slaveAdress, const uint8_t startRegister, uint8_t * buffer, const uint8_t n)
{
    selectRegister(slaveAdress, startRegister);
    auto transaction = bus.read(slaveAdress);
    transaction.read(buffer, n);
}