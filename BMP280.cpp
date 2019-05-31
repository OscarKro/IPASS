#include "BMP280.hpp"

BMP280::BMP280(hwlib::i2c_bus_bit_banged_scl_sda bus): bus(bus) {}

void BMP280::start()
{
  bus.write(adress);
}