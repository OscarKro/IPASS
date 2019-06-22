#ifndef ENVIRONMENTREADER_HPP
#define ENVIRONMENTREADER_HPP
/// @file

/// \brief
/// Environment reader (chip) class
/// \details
/// This abstract super class provides the functions needed for the KromWorks Weatherstation application to function. Read this documentation to understand how
/// these member functions needs to be implemented.
class EnvironmentReader
{
    public:
    /// \brief
    /// Function to read the temperature
    /// \details
    /// This abstract member function returns a signed 16 bits integer. If the return value is 2325, the application shows it's 23.25 degrees celsius.
    /// It's not possible to return 23.25 by itself as a float. If the chip you are using always returns in (for instance) 23 degrees celsius, then this function
    /// needs to multiply that number by a hundred before returning it.
    virtual int16_t readTemperature() = 0;

    /// \brief
    /// Function to read the pressure 
    /// \details
    /// This abstract member function returns an unsigned 32 bits integer. If the return value is 101845, the application shows it in 1018 hPa.
    /// It's not possible to return it in hPa, it has to be done in Pascal. If the chip you are using always returns in hPa, you can simply multiply
    /// this number by a hundred before returning it. If you don't want to read the pressure, this function needs to return 0.
    virtual uint32_t readPressure() = 0;

    /// \brief
    /// Function to read trimming or calculation paramaters
    /// \details
    /// This abstract member function reads paramaters if needed. Some chips like the BMP280 or 180 need certain paramaters hard programmed
    /// in their registers read, to calculate the temperature from the raw data.
    /// How you store these parameters or if you even use this function is up to you. The application only uses this function 
    /// once during the startup routine. If this is not needed or wanted, this function can return nothing.
    virtual void readParam() = 0;

    /// \brief
    /// Function to read the id of the chip
    /// \details
    /// This abstract member function reads the id of the chip and returns an unsigned 8 bits integer. In the case of the BMP280 chip, the id is 88.
    /// If the environment reader you're using has another id number, the startup() function in the application needs to know this. To do so, you have to change
    /// the uint8_t id variable in the KromWorksWeatherStation.hpp (private) to the corresponding number. If your environment chip doesn't have an id number,
    /// you can return 88 from this function. The application uses this function once during the start up routine to make sure a chip is connected.
    virtual uint8_t readId() = 0;

    /// \brief
    /// Function to reset the chip
    /// \details
    /// This abstract member function implements a soft reset for the chip. To see how your chip is reset, read the datasheet. If the chip cannot reset
    /// you can return immediately from this function. This function is only used once during the start up routine from the application.
    virtual void reset() = 0;
};
#endif //ENVIRONMENTREADER_HPP