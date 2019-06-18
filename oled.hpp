#ifndef OLED_HPP
#define OLED_HPP
#include "hwlib.hpp"
class oled
{
private:
    hwlib::i2c_bus &bus;
    hwlib::glcd_oled display;

    hwlib::font_default_16x16 font;
    hwlib::terminal_from window;

public:
    oled(hwlib::i2c_bus &bus);
    void resetCursor(uint8_t x, uint8_t y);
    void clearScreen();
    void drawInt(const int8_t x);
    void drawInt(const uint16_t x);
    void drawByte(const uint8_t x);
    void drawText(const char *s);
    void drawLine(const hwlib::xy& start, const hwlib::xy& end);
    void flush();
};

#endif //OLED_HPP
