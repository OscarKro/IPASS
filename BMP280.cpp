#include "BMP280.hpp"

BMP280::BMP280(hwlib::i2c_bus &bus) : bus(bus) {}

void BMP280::selectRegister(const uint8_t adress)
{
  auto transaction = bus.write(adresses::i2cAdress);
  transaction.write(adress);
}

uint8_t BMP280::readTemp()
{
  selectRegister(adresses::tempAdress1);
  uint8_t msb = bus.read(adresses::i2cAdress).read_byte();
  uint8_t lsb = bus.read(adresses::i2cAdress).read_byte();
  uint8_t xlsb = bus.read(adresses::tempAdress3).read_byte();
  uint32_t total = msb << 8;
  total |= lsb;
  total <<= 8;
  total |= xlsb;
  hwlib::cout << "waarde totaal: " << total;
  return total;
}

uint8_t BMP280::readId()
{
  selectRegister(adresses::idAdress);
  uint8_t id = bus.read(adresses::i2cAdress).read_byte();
  hwlib::cout << hwlib::hex << id << hwlib::endl;
  return id;
}

void BMP280::setMode(char ch)
{
  auto transaction = bus.write(adresses::i2cAdress);
  transaction.write(adresses::modeAdress);
  uint8_t mode = 0b00000000;
  if (ch == 's')
  {
    mode = 0b00000000;
    transaction.write(mode);
  }
  else if (ch == 'n')
  {
    mode = 0b00000011;
    transaction.write(mode);
  }
  else if (ch == 'f')
  {
    mode = 0b00000001;
    transaction.write(mode);
  }
  hwlib::cout << "mode gezet in: " << mode << hwlib::endl;
}

// long signed int t_fine;
// double bmp280_compensate_T_double(long signed int adc_T)
// {
// double var1, var2, T;
// var1 = (((double)adc_T)/16384.0 – ((double)dig_T1)/1024.0) * ((double)dig_T2);
// var2 = ((((double)adc_T)/131072.0 – ((double)dig_T1)/8192.0) *
// (((double)adc_T)/131072.0 – ((double) dig_T1)/8192.0)) * ((double)dig_T3);
// t_fine = (long signed int)(var1 + var2);
// T = (var1 + var2) / 5120.0;
// return T;
// }
// Returns pressure