#include "BMP280.hpp"

BMP280::BMP280(hwlib::pin_oc& scl, hwlib::pin_oc& sda): bus(scl,sda) {}

void BMP280::select()
{
  bus.write(adress).write(0);//schrijf ik hier het adres met een 1 erachteraan om te schrijven? waarom kan dit niet? Wouter vragen!
}