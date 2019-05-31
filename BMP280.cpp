#include "BMP280.hpp"

BMP280::BMP280(hwlib::pin_oc& scl, hwlib::pin_oc& sda): bus(scl,sda) {}

int8_t BMP280::readTemp()
{
  bus.write(adress);
  int8_t x = 10; //placeholder
  return x;
}