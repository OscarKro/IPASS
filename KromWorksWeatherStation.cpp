#include "KromWorksWeatherStation.hpp"

Weatherstation::Weatherstation(BMP280 &BMP280, oled &display) : chip(BMP280), display(display) {}

void Weatherstation::startUp()
{
    display.clearScreen();
    display.resetCursor();
    display.drawText(" Hola!");
    hwlib::wait_ms(2000);
    display.resetCursor();
    display.drawText("starting\n");
    chip.reset();
    hwlib::wait_ms(2000);
    display.drawText("done!");
    hwlib::wait_ms(1000);
    display.resetCursor();
    display.clearScreen();
    display.drawText("reading\n");
    display.drawText("param...");
    chip.readTempParam();
    chip.readPressParam();
    hwlib::wait_ms(2000);
    display.resetCursor();
    display.clearScreen();
    display.drawText("setting\n");
    display.drawText("mode...");
    chip.setMode();
    hwlib::wait_ms(2000);
    display.resetCursor();
    display.clearScreen();
    display.drawText(" done!");
    hwlib::wait_ms(2000);
    display.resetCursor();
    display.clearScreen();
    display.drawText("welcome!");
    hwlib::wait_ms(2000);
}