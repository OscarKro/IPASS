#include "BMP280.hpp"

BMP280::BMP280(hwlib::i2c_bus_bit_banged_scl_sda bus): bus(bus) {}

void BMP280::start()
{
  bus.primitives.write_start();
  bus.write(generalAdress);
  bus.write(tempAdress1);
  bus.primitives.write_start();
  bus.read(generalAdress);
  uint8_t x = bus.read(tempAdress1);
  bus.primitives.write_nack();
  bus.primitives.write_stop();
}//waarom zoveel fout meldingen, ga naar wouter!