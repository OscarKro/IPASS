#ifndef BMP280_HPP
#define BMP280_HPP
/// @file
#include "BoschBM.hpp"
#include "EnvironmentReader.hpp"
/// \brief
/// BMP280 Class
/// \details
/// This BMP280 class implements all functions and variables needed to control and read a BMP280 chip from Bosch. The class inherits from 
/// the EnvironmentReader and BoschBM class. It has a data struct and bool under private. Everything else is public.
class BMP280 : public EnvironmentReader, BoschBM
{
private:

    /// \brief
    /// Struct with all data
    /// \details
    /// This private struct has all the data. All member functions that read or calculate (except calculatePress()) place their data in this struct.
    /// this data is can then be used again by other member functions. It can also be read outside this class by calling data.returnDataStruct().
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

    /// \brief
    /// Register adress enumerator
    /// \details
    /// This public enumerator houses all the register adresses and the slave adress (i2cRegister). All adresses are stored in a constant unsigned 
    /// 8 bits format.
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

    /// \brief
    /// Precision mode enumerator
    /// \details
    /// This enumerator houses all the precision modes used by the setMode() function. The higher you set the oversampeling, the more accurate the 
    /// measurment will be. The Bosch BMP280 datasheet advises to set the oversampling to one time for a weatherstation. All oversampling modes are 
    /// stored in a constant unsigned 8 bits format.
    enum class precision : const uint8_t
    {
        off = 0b00000001,
        oversampelingOneTime = 0b00100101,
        oversampelingTwoTimes = 0b01001001,
        oversampelingFourTimes = 0b01101101,
        oversampelingEightTimes = 0b10010001
    };

    /// \brief
    /// Constructor for the BMP280 class
    /// \details
    /// This constructor requires a hwlib i2c bus object to function. It gives this by reference to the super class BoschBM. Read the hwlib documentation for
    /// details on the i2c bus object.
    BMP280(hwlib::i2c_bus &bus);

    /// \brief
    /// Overwritten read temperature function
    /// \details
    /// To read the temperature or pressure from the chip, this member function first calls the setMode(const uint8_t mode) function every time it is called. 
    /// It then reads all temperature and pressure registers using the readPTRegisters() function. After this it calculates the temperature using 
    /// the calculateTemp() function and returns the temperature in a unsigned 16 bits format. 
    /// See the EnvironmentReader class, and this class documentation for further details on this.
    int16_t readTemperature() override;

    /// \brief
    /// Overwritten read Pressure function
    /// \details
    /// This member function returns the pressure in an unsigned 32 bits format. It only calls the calculateTemp() function. To call this function effectively, 
    /// you will always have to call the readTemperature() function first. It needs the temperature to calculate the pressure. For further details 
    /// how this is used, see the EnvironmentReader class documentation.
    uint32_t readPressure() override;

    /// \brief
    /// Overwritten Read Paramaters function
    /// \details
    /// This member function reads all the trimming paramaters needed to calculate the temperature and pressure from the raw data. It only does this when
    /// the bool paramRead is false. It then sets it to true. It stores all the values in the data struct.
    void readParam() override;
    
    /// \brief 
    /// Overwritten reset function
    /// \details
    /// This member function resets the chip by writing 0xB6 to the resetRegister. It uses writeSingleByte() from the BoschBM class.
    void reset() override;
    /// \brief
    /// Overwritten read ID function
    /// \details
    /// This member function reads the id (88) from the BMP280 chip. It uses the readSingleByte() function from the BoschBM class. See the documentation
    /// of this class and the Environment class for more details.
    uint8_t readId() override;

    /// \brief
    /// Function to read temperature and pressure registers
    /// \details
    /// This member function reads the Temperature and pressure registers using the burstRead() function from the BoschBM class. It stores the results in the
    /// data struct. Raw temperature data is stored in data.totalTempBin and raw pressure data is stored in data.totalPressBin.
    void readPTregisters();

    /// \brief
    /// Function to set the mode of the chip
    /// \details
    /// This member function is used to set the resolution and filter modes of the chip. By default the filter is used one time each measurement for the pressure data.
    /// this filter is necessary to extract sudden increases in pressure from the data. For example the closing of a door. See the datasheet for further
    /// details on this. The Resolution for the measurement is given in the parameter mode. You can find the oversampling (resolution) modes in the 
    /// enumerator "precision". The setMode function has to be called again every measurement. The function uses the writeSingleByte() member function
    /// from the BoschBM class. See the BoschBM class documentation for further details on that.
    void setMode(const uint8_t mode);

    /// \brief
    /// Function to calculate the temperature
    /// \details
    /// This member function calculates the temperature from the raw data. It gets the raw data from the data struct (data.totalTempBin). For details 
    /// how this is calculated, see the datasheet from the Bosch BMP280. It returns the data in a signed 16 bits format. If the data gives the number
    /// 2345, it is 23.45 degrees celcius.
    int16_t calculateTemp();

    /// \brief
    /// Function to calculate the pressure
    /// \details
    /// This member function calculates the pressure from the raw data. It gets the raw data from the data struct (data.totalPressBin). For details
    /// how this is calculated, see the datasheet from the Bosch BMP280. It returns the data in an usnigned 32 bits format. If the data gives the number
    /// 101800, it is 1018 hPa pressure.
    uint32_t calculatePress();

    /// \brief
    /// Function to return the data struct
    /// \details
    /// This member function returns a copy of the data struct to be used outside this class.
    BMPData returnDataStruct();
};
#endif //BMP280_HPP
