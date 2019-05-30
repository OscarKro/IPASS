#include "bmpoled.hpp"
//bmp slave adress = 0b1110110 (0x76)
bmpoled::bmpoled(hwlib::target::pin_oc sclOled, hwlib::target::pin_oc sdaOled)
    : sclOled(sclOled), sdaOled(sdaOled), bus(sclOled, sdaOled) ,display(bus, 0x3c), window(display, font) {}

void bmpoled::drawTemp(const int x)
{
    window << "\n" << x << " C";
    display.flush();
    hwlib::wait_ms(100);
    display.flush();
}

void bmpoled::readBMP()
{
//hier verder gaan. bus.write doen? ambiqious error? Kennelijk meerdere writes bekend? vragen?
}