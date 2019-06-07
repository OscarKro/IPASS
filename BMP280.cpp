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

int32_t *BMP280::readPTRegisters()
{
  //omdat het hele spul wordt uitgelezen in 20 bit format, en bij xlsb de rechtse 4 mee doen, en de rest niet.
  //wordt xlsb 4 keer naar rechts geshift en dan meegenomen in het totaal;
  selectRegister(adresses::pressureAdress1);
  auto transaction = bus.read(adresses::i2cAdress);
  int8_t msbPress = transaction.read_byte();
  int8_t lsbPress = transaction.read_byte();
  int8_t xlsbPress = transaction.read_byte();
  int8_t msbTemp = transaction.read_byte();
  int8_t lsbTemp = transaction.read_byte();
  int8_t xlsbTemp = transaction.read_byte();
  transaction.~i2c_read_transaction();

  int32_t totalPress = 0;
  totalPress |= msbPress;
  totalPress <<= 8;
  totalPress |= lsbPress;
  totalPress <<= 4;
  xlsbPress >>= 4;
  totalPress |= xlsbPress;

  int32_t totalTemp = 0;
  totalTemp |= msbTemp;
  totalTemp <<= 8;
  totalTemp |= lsbTemp;
  totalTemp <<= 4;
  xlsbTemp >>= 4;
  totalTemp |= xlsbTemp;

  static int32_t total[2] = {totalPress, totalTemp};

  return total;
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
  uint8_t mode = 0b00100101; //temperature oversampeling = 1x, pressure oversampling = 1x, forced mode
  uint8_t config = 0b00000000;
  //uint8_t config = 0b00000000;
  writeByte(adresses::ctrl_measAdress, mode);
  writeByte(adresses::configAdress, config);
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