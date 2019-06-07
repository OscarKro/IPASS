#include "BMP280.hpp"

BMP280::BMP280(hwlib::i2c_bus &bus) : bus(bus) {}

void BMP280::selectRegister(const uint8_t adress)
{
  auto transaction = bus.write(adresses::i2cAdress);
  transaction.write(adress);
  transaction.~i2c_write_transaction();
}

uint8_t BMP280::readByte()
{
  return bus.read(adresses::i2cAdress).read_byte();
}

void BMP280::writeByte(const uint8_t adress, const uint8_t byte)
{
  auto transaction = bus.write(adresses::i2cAdress);
  transaction.write(adress);
  transaction.write(byte);
  transaction.~i2c_write_transaction();
}

uint8_t BMP280::readPTRegisters(uint8_t x)
{
  //omdat het hele spul wordt uitgelezen in 20 bit format, en bij xlsb de rechtse 4 mee doen, en de rest niet.
  //wordt xlsb 4 keer naar rechts geshift en dan meegenomen in het totaal;
  selectRegister(adresses::pressureAdress1);
  uint8_t msbPress = readByte();
  uint8_t lsbPress = readByte();
  uint8_t xlsbPress = readByte();
  uint8_t msbTemp = readByte();
  uint8_t lsbTemp = readByte();
  uint8_t xlsbTemp = readByte();

  uint32_t totalPress = msbPress << 8;
  totalPress |= lsbPress;
  totalPress <<= 4;
  xlsbPress >>= 4;
  totalPress |= xlsbPress;

  uint32_t totalTemp = msbTemp << 8;
  totalTemp |= lsbTemp;
  totalTemp <<= 4;
  xlsbTemp >>= 4;
  totalPress |= xlsbTemp;

  if (x == 0)
  {
    hwlib::cout << "pressure: " << hwlib::bin << totalPress << hwlib::endl;
    return totalPress;
  }
  else if (x == 1)
  {
    hwlib::cout << "temperature: " << hwlib::bin << totalTemp << hwlib::endl;
    return totalTemp;
  }
  else
  {
    hwlib::cout << "no good mode selected" << hwlib::endl;
    return 1;
  }
}

uint8_t BMP280::readId()
{
  selectRegister(adresses::idAdress);
  uint8_t id = readByte();
  hwlib::cout << hwlib::hex << id << hwlib::endl;
  return id;
}

void BMP280::setMode()
{
  //eerste 3 bits = temp, 2e 3 bits = pressure laatste 2 is mode.
  uint8_t mode = 0b11110101; //temperature oversampeling = 16x, pressure oversampling = 16x, forced mode
  //uint8_t config = 0b00000000;
  writeByte(adresses::ctrl_measAdress, mode);
}

void BMP280::reset()
{
  writeByte(adresses::resetAdress, 0xB6);
  hwlib::cout << "reset complete" << hwlib::endl;
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