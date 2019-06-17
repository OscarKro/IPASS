#include "KromWorksWeatherStation.hpp"

Weatherstation::Weatherstation(BMP280 &BMP280, oled &display, hwlib::pin_in &button) : chip(BMP280), display(display), button(button) {}

//one complete measurementCycle (read temperature, read pressure, calculate both, and write it to screen).
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
    hwlib::wait_ms(1000);
    display.clearScreen();
    display.resetCursor(0, 1);
    display.drawText("Weather\nstation");
    hwlib::wait_ms(1000);

    display.resetCursor(0, 1);
    display.clearScreen();
    display.drawText("starting\n");
    chip.reset();
    display.drawText("done!");
    hwlib::wait_ms(500);

    display.resetCursor(0, 1);
    display.clearScreen();
    display.drawText("connect\nchip");
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
        display.drawText("error\nrestart");
        hwlib::wait_ms(500);
        return startUp();
    }

    display.resetCursor(0,1);
    display.clearScreen();
    display.drawText("checking\nbutton");
    hwlib::wait_ms(500);
    if(!button.read())
    {
        display.clearScreen();
        display.resetCursor(0,1);
        display.drawText("succes");
        hwlib::wait_ms(500);
    }
    else
    {
        display.clearScreen();
        display.resetCursor(0,1);
        display.drawText("error\nrestart");
        hwlib::wait_ms(500);
        return startUp();
    }

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
    hwlib::wait_ms(1000);
}

//function to call one measurement cycle, with wait time in minutes(to be put in a while loop).
//this function also listens to the button. If the button is pressed, it shows the chart, if it is then pressed
//again, it continues the measurement cycle.
void Weatherstation::measurementWithInterval(uint8_t timeInMinutes)
{
    uint32_t measCounter = 0;
    uint8_t buttonWaitTime = 100;
    uint8_t buttonCheckTime = 10;

    if (firstMeasurement)
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
                    display.clearScreen();
                    display.resetCursor(0, 1);
                    display.drawInt(chip.returnData().realTemp);
                    display.drawText(" C\n");
                    display.drawInt(chip.returnData().realPress);
                    display.drawText(" hPa");
                    break;
                }
                hwlib::wait_ms(buttonCheckTime);
            }
        }
        else if (!firstMeasurement && measCounter == (timeInMinutes * 6000))
        {
            measurementCyle();
            return;
        }
        hwlib::wait_ms(buttonCheckTime);
    }
}
//function to write a chart to the oled screen using the temperature data from 30 minutes.
void Weatherstation::drawChart()
{
    display.clearScreen();
    hwlib::xy topLeftPoint(0, 0);
    hwlib::xy zeroDegreesPoint(127, 32);
    display.drawLine(topLeftPoint, hwlib::xy(topLeftPoint.x, zeroDegreesPoint.y + 32));
    display.drawLine(hwlib::xy(topLeftPoint.x, zeroDegreesPoint.y), hwlib::xy(zeroDegreesPoint.x, zeroDegreesPoint.y));
    uint16_t xPoint = 4;
    for (uint8_t i : data.tempArray)
    {
        if (i == 0)
        {
            display.drawLine(hwlib::xy(topLeftPoint.x, zeroDegreesPoint.y - data.tempArray[i]), hwlib::xy(xPoint, zeroDegreesPoint.y - data.tempArray[i]));
        }
        else
        {
            display.drawLine(hwlib::xy(xPoint - 4, zeroDegreesPoint.y - data.tempArray[i - 1]), hwlib::xy(xPoint, zeroDegreesPoint.y -
                                                                                                                      data.tempArray[i]));
        }
        xPoint += 4;
    }
}