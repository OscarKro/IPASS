#ifndef BMP280_HPP
#define BMP280_HPP
#include "BoschBM.hpp"
#include "EnvironmentReader.hpp"
class BMP280 : public EnvironmentReader, BoschBM
{
private:

    struct BMPData
    {
        int32_t totalPressBin = 0;
        int32_t totalTempBin = 0;
        uint16_t dig_t1 = 0;
        int16_t dig_t2 = 0;
        int16_t dig_t3 = 0;
        uint16_t dig_p1 = 0;
        int16_t dig_p2 = 0;
        int16_t dig_p3 = 0;
        int16_t dig_p4 = 0;
        int16_t dig_p5 = 0;
        int16_t dig_p6 = 0;
        int16_t dig_p7 = 0;
        int16_t dig_p8 = 0;
        int16_t dig_p9 = 0;
        int32_t t_fine = 0;
    };

    BMPData data;
    bool paramRead = 0;

public:

    enum class reg : const uint8_t
    {
        resetRegister = 0xE0,
        idRegister = 0xD0,
        statusRegister = 0xF3,
        ctrl_measRegister,
        configRegister,
        i2cRegister,
        pressureRegister1,
        pressureRegister2,
        pressureRegister3,
        tempRegister1,
        tempRegister2,
        tempRegister3,

        dig_t1Register1 = 0x88,
        dig_t1Register2,
        dig_t2Register1,
        dig_t2Register2,
        dig_t3Register1,
        dig_t3Register2,
        dig_p1Register1,
        dig_p1Register2,
        dig_p2Register1,
        dig_p2Register2,
        dig_p3Register1,
        dig_p3Register2,
        dig_p4Register1,
        dig_p4Register2,
        dig_p5Register1,
        dig_p5Register2,
        dig_p6Register1,
        dig_p6Register2,
        dig_p7Register1,
        dig_p7Register2,
        dig_p8Register1,
        dig_p8Register2,
        dig_p9Register1,
        dig_p9Register2
    };

    enum class precision : const uint8_t
    {
        off = 0b00000001,
        oversampelingOneTime = 0b00100101,
        oversampelingTwoTimes = 0b01001001,
        oversampelingFourTimes = 0b01101101,
        oversampelingEightTimes = 0b10010001
    };

    BMP280(hwlib::i2c_bus &bus);
    int16_t readTemperature();
    uint32_t readPressure();
    void readParam();
    void reset();
    uint8_t readId();
    void readPTregisters();
    void setMode(const uint8_t mode);
    int16_t calculateTemp();
    uint32_t calculatePress();
    BMPData returnDataStruct();
};
#endif //BMP280_HPP
