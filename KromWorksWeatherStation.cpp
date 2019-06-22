#include "KromWorksWeatherStation.hpp"

//constructor from weatherstation. Requires a BMP280 object, an oled class display and a pin_in object
Weatherstation::Weatherstation(EnvironmentReader &chip, WeatherStationDisplay &display, hwlib::pin_in &button) : chip(chip), display(display), button(button) {}

//function to push back the data from the chip into their respective data array
void Weatherstation::WeatherstationData::pushBack(int16_t dataTemp, uint32_t dataPress)
{
    if (n < maxnMeas)
    {
        tempArray[n] = dataTemp;
        pressArray[n] = dataPress;
        n++;
    }
    else if (n >= maxnMeas)
    {
        n = 0;
        tempArray[n] = dataTemp;
        pressArray[n] = dataPress;
        n++;
    }
}
//function to wipe the last item in both the data arrays
void Weatherstation::WeatherstationData::popBack()
{
    tempArray[n] = 0;
    pressArray[n] = 0;
    n--;
}
//function to wipe all the data from both the data arrays
void Weatherstation::WeatherstationData::wipeData()
{
    for (uint8_t i = 0; i < n; i++)
    {
        tempArray[i] = 0;
        pressArray[i] = 0;
    }
}

//standard startup routine for the chip, led screen and the button.
//it resets, checks if the BMP280 are properly connected and starts the first read of all the registers
// and it outputs everything to the led screen.
//the first read of the registers had to be done to make sure it has proper values in those registers. that's
//because the reset function resets those registers to some random value.
void Weatherstation::startUp()
{
    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawText("KromWork\n");
    display.flush();
    hwlib::wait_ms(1000);
    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawText("Weather\nstation");
    display.flush();
    hwlib::wait_ms(1000);

    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawText("starting\n");
    display.flush();
    chip.reset();
    display.drawText("done!");
    display.flush();
    hwlib::wait_ms(500);
    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawText("connect\nchip");
    display.flush();
    hwlib::wait_ms(500);
    if (chip.readId() == id)
    {
        display.clearScreen();
        display.resetCursor(0, 1);
        display.drawText("succes");
        display.flush();
        hwlib::wait_ms(500);
    }
    else
    {
        display.clearScreen();
        display.resetCursor(0, 1);
        display.drawText("error\nrestart");
        display.flush();
        hwlib::wait_ms(500);
        return startUp();
    }

    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawText("checking\nbutton");
    display.flush();
    hwlib::wait_ms(500);
    if (!button.read())
    {
        display.clearScreen();
        display.resetCursor(0, 1);
        display.drawText("succes");
        display.flush();
        hwlib::wait_ms(500);
    }
    else
    {
        display.clearScreen();
        display.resetCursor(0, 1);
        display.drawText("error\nrestart");
        display.flush();
        hwlib::wait_ms(500);
        return startUp();
    }
    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawText("reading\n");
    display.drawText("param...");
    display.flush();
    chip.readParam();
    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawText("succes");
    display.flush();
    hwlib::wait_ms(500);
    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawText("welcome!");
    display.flush();
    hwlib::wait_ms(1000);
}

void Weatherstation::drawTempAndPress()
{
    auto temp = chip.readTemperature();
    auto press = chip.readPressure();
    int8_t tempMSB = temp / 100;
    uint8_t tempLSB = temp - (temp / 100);
    data.pushBack(temp, press);
    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawInt(tempMSB);
    display.drawText(".");
    display.drawInt(tempLSB);
    display.drawText(" C\n");
    if (press > 0)
    {
        display.drawInt((uint16_t)(press / 100));
        display.drawText(" hPa");
    }
    display.flush();
}
//function to call one measurement cycle, with wait time in minutes(to be put in a while loop).
//this function also listens to the button. If the button is pressed, it shows the chart, if it is then pressed
//again, it continues the measurement cycle.
void Weatherstation::measurementWithInterval(uint8_t timeInMinutes)
{
    uint32_t measCounter = 0;
    uint32_t timeInterval = timeInMinutes * 6000;
    uint8_t buttonWaitTime = 100;
    uint8_t buttonCheckTime = 10;

    if (firstMeasurement)
    {
        firstMeasurement = 0;
        drawTempAndPress();
    }

    while (true)
    {
        measCounter++;
        if (button.read())
        {
            drawChart();
            hwlib::wait_ms(buttonWaitTime);
            while (true)
            {
                if (button.read())
                {
                    drawTempAndPress();
                    break;
                }
                hwlib::wait_ms(buttonCheckTime);
            }
        }
        else if (!firstMeasurement && measCounter == timeInterval)
        {
            drawTempAndPress();
            return;
        }
        hwlib::wait_ms(buttonCheckTime);
    }
}

//function to write a chart to the oled screen using the temperature data from 30 minutes.
void Weatherstation::drawChart()
{
    uint16_t xPoint = 0;
    display.clearScreen();
    hwlib::xy topLeftPoint(0, 0);
    hwlib::xy zeroDegreesPoint(127, 32);
    display.drawLine(topLeftPoint, hwlib::xy(topLeftPoint.x, zeroDegreesPoint.y + 32));
    display.drawLine(hwlib::xy(topLeftPoint.x, zeroDegreesPoint.y), hwlib::xy(zeroDegreesPoint.x, zeroDegreesPoint.y));
    for (uint8_t i = 0; i < data.maxnMeas; i++)
    {
        if (i == 0)
        {
            display.drawLine(hwlib::xy(topLeftPoint.x, zeroDegreesPoint.y - (data.tempArray[i] / 100)), hwlib::xy(xPoint, zeroDegreesPoint.y - (data.tempArray[i] - 100)));
        }
        else
        {
            display.drawLine(hwlib::xy(xPoint - 4, zeroDegreesPoint.y - (data.tempArray[i - 1] / 100)), hwlib::xy(xPoint, zeroDegreesPoint.y -
                                                                                                                              (data.tempArray[i] / 100)));
        }
        xPoint += 4;
    }
    display.flush();
}