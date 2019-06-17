#include "KromWorksWeatherStation.hpp"

Weatherstation::Weatherstation(BMP280 &BMP280, oled &display) : chip(BMP280), display(display) {}

//one complete measurementCycle (read temperature, read pressure, calculate both, and write it to screen)
void Weatherstation::measurementCyle()
{
    chip.setMode();
    chip.readPTRegisters();
    chip.calculateTemp();
    chip.calculatePress();
    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawInt(chip.returnData().realTemp);
    display.drawText(" C\n");
    display.drawInt(chip.returnData().realPress);
    display.drawText(" hPa");
    data.pushBack(chip.returnData().realTemp, chip.returnData().realPress);
}

//standard startup routine for both the chip and the led screen
void Weatherstation::startUp()
{
    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawText("KromWork\n");
    hwlib::wait_ms(1000);
    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawText("Weather\n");
    display.drawText("Station");
    hwlib::wait_ms(1000);

    display.resetCursor(0, 1);
    display.clearScreen();
    display.drawText("connect\n");
    display.drawText("chip");
    hwlib::wait_ms(500);
    if (chip.readId() == 1)
    {
        display.clearScreen();
        display.resetCursor(0, 1);
        display.drawText("succes");
        hwlib::wait_ms(500);
    }
    else
    {
        display.clearScreen();
        display.resetCursor(0, 1);
        display.drawText("error\n");
        display.drawText("restart");
        hwlib::wait_ms(500);
        startUp();
    }

    display.resetCursor(0, 1);
    display.clearScreen();
    display.drawText("starting\n");
    chip.reset();
    display.drawText("done!");
    hwlib::wait_ms(1000);

    display.resetCursor(0, 1);
    display.clearScreen();
    display.drawText("reading\n");
    display.drawText("param...");
    chip.setMode();
    chip.readTempParam();
    chip.readPressParam();
    chip.readPTRegisters();
    chip.calculateTemp();
    chip.calculatePress();
    display.resetCursor(0, 1);
    display.clearScreen();
    display.drawText(" done!");
    hwlib::wait_ms(500);

    display.resetCursor(0, 1);
    display.clearScreen();
    display.drawText("welcome!");
    hwlib::wait_ms(2000);
}

//function to call one measurement cycle, with wait time (to be put in a while loop)
void Weatherstation::measurementWithInterval(uint16_t time)
{
    if (firstMeasurement == 1)
    {
        firstMeasurement = 0;
        display.clearScreen();
        display.resetCursor(0, 1);
        display.drawInt(chip.returnData().realTemp);
        display.drawText(" C\n");
        display.drawInt(chip.returnData().realPress);
        display.drawText(" hPa");
        data.pushBack(chip.returnData().realTemp, chip.returnData().realPress);
    }
    else
    {
        hwlib::wait_ms(time);
        measurementCyle();
    }
}
//function to write a chart to the oled screen using the temperature data from 30 minutes
void Weatherstation::drawChart()
{
    display.clearScreen();
    hwlib::xy topLeftPoint(0, 0);
    hwlib::xy zeroDegreesPoint(127, 32);
    display.drawLine(topLeftPoint, hwlib::xy(topLeftPoint.x, zeroDegreesPoint.y + 32));
    display.drawLine(hwlib::xy(topLeftPoint.x, zeroDegreesPoint.y), hwlib::xy(zeroDegreesPoint.x, zeroDegreesPoint.y));
    uint16_t xPoint = 4;
    for (uint8_t i = 0; i < data.maxnMeas; i++)
    {
        if (i == 0)
        {
            display.drawLine(hwlib::xy(topLeftPoint.x, zeroDegreesPoint.y), hwlib::xy(xPoint, zeroDegreesPoint.y - data.tempArray[i]));
        }
        else
        {
            display.drawLine(hwlib::xy(xPoint - 10, zeroDegreesPoint.y - data.tempArray[i - 1]), hwlib::xy(xPoint, zeroDegreesPoint.y -
                                                                                                                       data.tempArray[i]));
        }
        xPoint += 4;
    }
}
