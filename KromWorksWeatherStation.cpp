#include "KromWorksWeatherStation.hpp"

Weatherstation::Weatherstation(BMP280 &BMP280, oled &display) : chip(BMP280), display(display) {}

void Weatherstation::startUp()
{
    //standard startup routine for both the chip and the led screen
    display.clearScreen();
    display.resetCursor();
    display.drawText(" Hola!");
    hwlib::wait_ms(2000);
    display.resetCursor();
    display.drawText("starting\n");
    chip.reset();
    display.drawText("done!");
    hwlib::wait_ms(1000);
    display.resetCursor();
    display.clearScreen();
    display.drawText("reading\n");
    display.drawText("param...");
    chip.setMode();
    chip.readTempParam();
    chip.readPressParam();
    chip.readPTRegisters();
    chip.calculateTemp();
    chip.calculatePress();
    display.resetCursor();
    display.clearScreen();
    display.drawText(" done!");
    hwlib::wait_ms(500);
    display.resetCursor();
    display.clearScreen();
    display.drawText("fetching\n");
    display.drawText("id...");
    chip.readId();
    display.resetCursor();
    display.clearScreen();
    display.drawText("id: ");
    display.drawByte(chip.returnData().id);
    hwlib::wait_ms(500);
    display.resetCursor();
    display.clearScreen();
    display.drawText("welcome!");
    hwlib::wait_ms(2000);
}

void Weatherstation::measurementCyle()
{
    //one complete measurementCycle (read temperature, read pressure, calculate both, and write it to screen)
    chip.setMode();
    chip.readPTRegisters();
    chip.calculateTemp();
    chip.calculatePress();
    display.clearScreen();
    display.resetCursor();
    display.drawInt32T(chip.returnData().realTemp);
    display.drawText(" C\n");
    display.drawUint32T(chip.returnData().realPress);
    display.drawText(" hPa");
}

void Weatherstation::intervalMeasurement(uint16_t time)
{
    //function to call one measurement cycle, with wait time (to be put in a while loop)
    measurementCyle();
    hwlib::wait_ms(time);
}