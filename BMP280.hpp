#ifndef BMP280_HPP
#define BMP280_HPP
#include "hwlib.hpp"
#include "BoschBM.hpp"
class BMP280 : public BoschBM
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
        int16_t realTempMS = 0;
        int16_t realTempLS = 0;
        uint16_t realPress = 0;
    };

    struct precisionMode
    {
        const uint8_t off = 00000001;
        const uint8_t oversampelingOneTime = 0b00100101;
        const uint8_t oversampelingTwoTimes = 0b01001001;
        const uint8_t oversampelingFourTimes = 0b01101101;
        const uint8_t oversampelingEightTimes = 0b10010001;
    };

    BMPData data;
    precisionMode mode;

public:

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
        dig_t2Adress1,
        dig_t2Adress2,
        dig_t3Adress1,
        dig_t3Adress2,
        dig_p1Adress1,
        dig_p1Adress2,
        dig_p2Adress1,
        dig_p2Adress2,
        dig_p3Adress1,
        dig_p3Adress2,
        dig_p4Adress1,
        dig_p4Adress2,
        dig_p5Adress1,
        dig_p5Adress2,
        dig_p6Adress1,
        dig_p6Adress2,
        dig_p7Adress1,
        dig_p7Adress2,
        dig_p8Adress1,
        dig_p8Adress2,
        dig_p9Adress1,
        dig_p9Adress2,
    };

    BMP280(hwlib::i2c_bus &bus);
    void readPTregisters();
    void readTempParam();
    void readPressParam();
    bool readId();
    void setMode(const uint8_t mode);
    void reset();
    void calculateTemp();
    void calculatePress();
    BMPData returnDataStruct();
    precisionMode returnModeStruct();
};
#endif //BMP280_HPP
