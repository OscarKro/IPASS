#ifndef ENVIRONMENTREADER_HPP
#define ENVIRONMENTREADER_HPP
#include "hwlib.hpp"
class EnvironmentReader
{
    public:
    virtual int16_t readTemperature() = 0;
    virtual uint32_t readPressure() = 0;
    virtual void readParam() = 0;
    virtual uint8_t readId() = 0;
    virtual void reset() = 0;
};
#endif //ENVIRONMENTREADER_HPP