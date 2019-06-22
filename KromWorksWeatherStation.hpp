#ifndef KROMWORKSWEATHERSTATION_HPP
#define KROMWORKSWEATHERSTATION_HPP
/// @file
#include "hwlib.hpp"
#include "EnvironmentReader.hpp"
#include "WeatherStationDisplay.hpp"

/// \brief
/// Weather station Application class
/// \details
/// This class is the application of a KromWorks WeatherStation. It includes the Hardware library from Wouter van Ooijen called "hwlib.hpp". All the 
/// hardware communication is done in that library and all rights for hwlib are reserved for Wouter van Ooijen.
class Weatherstation
{
private:
    EnvironmentReader &chip;
    WeatherStationDisplay &display;
    hwlib::pin_in &button;
    bool firstMeasurement = 1;
    
    /// \brief
    /// ID Number
    /// \details
    /// This unsigned byte is the id of the connected chip. It uses this number in the startUp() function to check if right chip or any chip is connected.
    /// If you connect another chip than the BMP280, this number has to be changed to the corresponding ID number. If the chip does not have an ID number
    /// you can let your implementation of the readID() function from the EnvironmentReader class return 88 so the weatherstation will think it has a BMP280.
    uint8_t id = 88;

    /// \brief
    /// Data struct
    /// \details
    /// This struct houses all the data necessary to draw a chart on the display from the temperature data. It has two arrays which can be filled up 
    /// with a maximum of 30 measurements. This can be increased by increasing the variable maxnMeas and the number between te brackets for both 
    /// the pressure and the temperature array.
    struct WeatherstationData
    {
        uint8_t maxnMeas = 30;
        uint8_t n = 0;
        int16_t tempArray[30] = {};
        //test array voor de grafiek
        // int16_t tempArray[30] = {
        //     -2000,
        //     -2000,
        //     -2000,
        //     -3100,
        //     -3100,
        //     -3100,
        //     -2000,
        //     -2000,
        //     -2000,
        //     -1000,
        //     -1000,
        //     -1000,
        //     0,
        //     0,
        //     0,
        //     500,
        //     500,
        //     500,
        //     1000,
        //     1000,
        //     1000,
        //     2000,
        //     2000,
        //     2000,
        //     3100,
        //     3100,
        //     3100,
        //     2000,
        //     2000,
        //     2000
        // };
        uint32_t pressArray[30] = {};

        /// \brief
        /// Push back function
        /// \details
        /// This member function of the data struct pushes back the measured data in the temperature and pressure arrays. If the number of measurements
        /// is higher than the max number of measurements (maxnMeas variable) it will put the data on the first index again and overwrite the old data.
        /// it requires signed 16 bits format temperature data and unsigned 32 bits format pressure data in its parameters.
        void pushBack(int16_t dataTemp, uint32_t dataPress);

        /// \brief
        /// Function to delete the last element
        /// \details
        /// This function deletes the latest entry in both the pressure and temperature arrays.
        void popBack();

        /// \brief
        /// Function to wipe all the data
        /// \details
        /// This function deletes all the data in both the pressure and temperature arrays.
        void wipeData();
    };

    WeatherstationData data;

public:
    /// \brief
    /// Constructor
    /// \details
    /// The constructor requires an object from the EnvironmentReader class, an object from the WeatherStationDisplay class and an object from
    /// the hwlib Pin_in class. 
    Weatherstation(EnvironmentReader &chip, WeatherStationDisplay &display, hwlib::pin_in &button);

    /// \brief
    /// Start up routine for the station
    /// \details
    /// This member function runs a start up routine for the chip and display. It does this to check if everything is working. If something is not working
    /// properly it will keep restarting untill it does. The startup routine uses member functions from the EnvironmentReader class and these need to be 
    /// implemented to support this function. The functions used are: readParam(), readId() and reset().  
    /// See the documentation off the EnvironmentReader class for more information.
    void startUp();

    /// \brief
    /// Function to draw temperature and pressure
    /// \details
    /// This function reads both the temperature and pressure and writes this to the display in the right format. It needs to recieve both in the 
    /// format described at the functions readTemperature() and readPressure() from the EnvironmentReader class. It devides these numbers with 100
    /// for the correct display of information. It uses functions from both the EnvironmentReader class, as the Weatherstation display class.
    void drawTempAndPress();

    /// \brief
    /// Function to do a measurment with interval
    /// \details
    /// This Function does a complete measurement and writes this to the display each "timeInMinutes". The parameter is the time you want to wait before
    /// the next measurement and display in minutes. This function also includes the use of the button. If you press the button, the drawChart() function 
    /// is called and it will draw a chart to the screen. If you then press it again, it will do a new measurement and draw this to the screen. Therefore, 
    /// this function (or rather the button) can also be used to force a measurement. If the function is showing the chart, it will not continue counting
    /// to the next measurement time. In other words, the function stops measuring untill the you pressed the button again in chart mode and return
    /// to normal mode. This function needs to be put in a while loop in the main. The function uses drawTempAndPress() and read() from hwlib.
    void measurementWithInterval(uint8_t timeInMinutes);

    /// \brief
    /// Function to draw a data chart
    /// \details
    /// This function draws a complete chart of the last 30 minutes of measurements. It uses the data from the data struct Weatherstationdata. If 
    /// for instance measurement minute 22 is not yet done, it will draw the line towards zero. If the temperature is above 32 or below -32 it will go
    /// off screen on small displays. The function uses drawLine() function from the WeatherStationDisplay class and xy objects from hwlib.
    void drawChart();
};

#endif //KROMWORKSWEATHERSTATION_HPP
