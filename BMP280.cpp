#include "BMP280.hpp"

BMP280::BMP280(hwlib::i2c_bus &bus) : bus(bus) {}

void BMP280::selectRegister(const uint8_t adress)
{
  bus.write(adresses::i2cAdress).write(adress);
}

uint8_t BMP280::readSingleByte(const uint8_t adress)
{
  bus.write(adresses::i2cAdress).write(adress);
  return bus.read(adresses::i2cAdress).read_byte();
}

void BMP280::writeSingleByte(const uint8_t adress, const uint8_t byte)
{
  auto transaction = bus.write(adresses::i2cAdress);
  transaction.write(adress);
  transaction.write(byte);
}

void BMP280::readPTRegisters(int32_t &totalPress, int32_t &totalTemp)
{
  setMode();
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
  int32_t _totalPress = 0;
  _totalPress |= msbPress;
  _totalPress <<= 8;
  _totalPress |= lsbPress;
  _totalPress <<= 4;
  xlsbPress >>= 4;
  _totalPress |= xlsbPress;

  int32_t _totalTemp = 0;
  _totalTemp |= msbTemp;
  _totalTemp <<= 8;
  _totalTemp |= lsbTemp;
  _totalTemp <<= 4;
  xlsbTemp >>= 4;
  _totalTemp |= xlsbTemp;

  totalPress = _totalPress;
  totalTemp = _totalTemp;
}

uint8_t BMP280::readId()
{
  uint8_t id = readSingleByte(adresses::idAdress);
  return id;
}

void BMP280::setMode()
{
  //eerste 3 bits = temp, 2e 3 bits = pressure laatste 2 is mode.
  const uint8_t mode = 0b00100101; //temperature oversampeling = 1x, pressure oversampling = 1x, forced mode
  const uint8_t config = 0b00000000;
  writeSingleByte(adresses::ctrl_measAdress, mode);
  writeSingleByte(adresses::configAdress, config);
  hwlib::wait_ms(10);
}

void BMP280::reset()
{
  writeSingleByte(adresses::resetAdress, 0xB6);
  hwlib::wait_ms(3000);//wait is nodig om de tijd te geven te resetten. anders is de uitkomst van de meting 10000... enz.
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