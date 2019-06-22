#ifndef BOSCHBM_HPP
#define BOSCHBM_HPP
/// @file
#include "hwlib.hpp"
/// \brief
/// Bosch BM class
/// \details
/// This super class has the basic functions needed to make a BM type environment chip from Bosch working. It reads and writes bits the way it's expected
/// by those chips. It has a reference to a bus from hwlib in it's private and all functions are public. It does not include the slave adress or register 
/// adresses, these need to be implemented in a subclass, heriting from this class. You can find these adresses in the corresponding datasheet. 
/// It includes the Hardware library from Wouter van Ooijen called "hwlib.hpp". All the hardware communication is done in that library and all rights 
/// for hwlib are reserved for Wouter van Ooijen.
class BoschBM
{
private:
    hwlib::i2c_bus &bus;

public:

    /// \brief
    /// Constructor
    /// \details
    /// This constructor requires a i2c bus object from hwlib, read the hwlib documentation for details on this class.
    BoschBM(hwlib::i2c_bus &bus);

    /// \brief
    /// Function to select a register
    /// \details
    /// This member function selects a register. If you want to read a register from a Bosch BMx chip, you first need to select the register to be able
    /// to do so. It first sends the slave adress and recieves an ACKS. It then sends the register adress, followed by an ACKS and a stop bit. You 
    /// do NOT need to call this function in any of the other functions from this class. This is already implemented. It requires a constant unsigned 
    /// 8 bit slave adress and a constant unsigned 8 bit register adress in its parameters.
    virtual void selectRegister(const uint8_t slaveAdress, const uint8_t registerAdress);

    /// \brief
    /// Function to read a single byte
    /// \details
    /// This member function reads a single byte from a given register and returns this in an unsigned 8 bit format. You do NOT need to select a
    /// register first. It requires a constant unsigned 8 bit slave adress and a constant unsigned 8 bit register adress in its parameters.
    virtual uint8_t readSingleByte(const uint8_t slaveAdress, const uint8_t registerAdress);

    /// \brief
    /// Function to write a single byte
    /// \details
    /// This member function writes a single byte to a given register. You do NOT need to select a register first.
    /// It requires a constant unsigned 8 bit format slave adress, a constant unsigned 8 bit format register adress and a constant unsigned 8 bits number
    /// (the byte you want to write) in its parameters.
    virtual void writeSingleByte(const uint8_t slaveAdress, const uint8_t registerAdress, const uint8_t byte);

    /// \brief
    /// Function to read multiple registers using auto increment.
    /// \details
    /// This member function reads multiple bytes from multiple registers. The Bosch BMx chips use auto increment to read multiple registers. this is
    /// for faster transmission of data and to be sure the temperature or pressure data is from one measurment only and not multiple. The adress
    /// numbers need to be successively in order to get the right data. The function puts each byte in an unsigned 8 bits array called buffer. The first byte 
    /// will be in position 0, the second in position 1 and so on. It does this n times. You do NOT need to select a register first. It requires a constant 
    /// unsigned 8 bits slave adress, a constant unsigned 8 bits adress from the first register in the row, an unsigned 8 bits array with n size and
    /// the n number of registers(bytes) you want to read. The size of the buffer and n needs to be equal.
    virtual void burstRead(const uint8_t slaveAdress, const uint8_t startRegister, uint8_t *buffer, const uint8_t n);
};
#endif //BOSCHBM_HPP