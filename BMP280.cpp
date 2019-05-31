#include "BMP280.hpp"

BMP280::BMP280(hwlib::pin_oc& scl, hwlib::pin_oc& sda): bus(scl,sda) {}

// void BMP280::select()
// {
//   bus.write_bit(1);

// }