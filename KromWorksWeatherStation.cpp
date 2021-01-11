#include "KromWorksWeatherStation.hpp"


Weatherstation::Weatherstation(EnvironmentReader &chip, WeatherStationDisplay &display, hwlib::pin_in &button) : chip(chip), display(display), button(button) {}


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

void Weatherstation::WeatherstationData::popBack()
{
    tempArray[n] = 0;
    pressArray[n] = 0;
    n--;
}

void Weatherstation::WeatherstationData::wipeData()
{
    for (uint8_t i = 0; i < n; i++)
    {
        tempArray[i] = 0;
        pressArray[i] = 0;
    }
}


void Weatherstation::startUp()
{
    display.clearScreen();
    display.resetCursor(0, 0);
    display.drawText("OK\nSystem\nDevelopment\n");
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
                    if (data.n == 0)
                    {
                        display.clearScreen();
                        display.resetCursor(0, 1);
                        display.drawInt((int8_t)(data.tempArray[0] / 100));
                        display.drawText(".");
                        display.drawInt((uint8_t)(data.tempArray[0] - (data.tempArray[0] / 100)));
                        display.drawText("\n");
                        if (data.pressArray[0] > 0)
                        {
                            display.drawInt((uint16_t)(data.pressArray[0] / 100));
                            display.drawText(" hPa");
                        }
                        display.flush();
                    }
                    else
                    {
                        display.clearScreen();
                        display.resetCursor(0, 1);
                        display.drawInt((int8_t)(data.tempArray[data.n-1] / 100));
                        display.drawText(".");
                        display.drawInt((uint8_t)(data.tempArray[data.n-1] - (data.tempArray[data.n-1] / 100)));
                        display.drawText("\n");
                        if (data.pressArray[0] > 0)
                        {
                            display.drawInt((uint16_t)(data.pressArray[data.n-1] / 100));
                            display.drawText(" hPa");
                        }
                        display.flush();
                    }
                    
                    break;
                }
                hwlib::wait_ms(buttonCheckTime);
            }
        }
        else if (!firstMeasurement && measCounter >= timeInterval)
        {
            drawTempAndPress();
            return;
        }
        hwlib::wait_ms(buttonCheckTime);
    }
}


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