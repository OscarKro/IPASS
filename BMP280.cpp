#include "BMP280.hpp"

BMP280::BMP280(hwlib::i2c_bus &bus) : bus(bus) {}

void BMP280::selectRegister(const uint8_t adress)
{
  bus.write(adresses::i2cAdress).write(adress);
}

uint8_t BMP280::readSingleByte(const uint8_t adress)
{
  selectRegister(adress);
  return bus.read(adresses::i2cAdress).read_byte();
}

void BMP280::writeSingleByte(const uint8_t adress, const uint8_t byte)
{
  auto transaction = bus.write(adresses::i2cAdress);
  transaction.write(adress);
  transaction.write(byte);
}

void BMP280::readPTRegisters()
{
  setMode();
  hwlib::wait_ms(10);
  selectRegister(adresses::pressureAdress1);
  auto transaction = bus.read(adresses::i2cAdress);
  data.totalPressBin |= transaction.read_byte();
  data.totalPressBin <<= 8;
  data.totalPressBin |= transaction.read_byte();
  data.totalPressBin <<= 4;
  data.totalPressBin |= transaction.read_byte() >> 4;

  data.totalTempBin |= transaction.read_byte();
  data.totalTempBin <<= 8;
  data.totalTempBin |= transaction.read_byte();
  data.totalTempBin <<= 4;
  data.totalTempBin |= transaction.read_byte() >> 4;
  // int8_t msbPress = transaction.read_byte();
  // int8_t lsbPress = transaction.read_byte();
  // int8_t xlsbPress = transaction.read_byte();
  // int8_t msbTemp = transaction.read_byte();
  // int8_t lsbTemp = transaction.read_byte();
  // int8_t xlsbTemp = transaction.read_byte();
  // xlsbPress >>= 4;
  // xlsbTemp >>= 4;

  // data.totalPressBin |= msbPress;
  // data.totalPressBin <<= 8;
  // data.totalPressBin |= lsbPress;
  // data.totalPressBin <<= 4;
  // data.totalPressBin |= xlsbPress;

  // data.totalTempBin |= msbTemp;
  // data.totalTempBin <<= 8;
  // data.totalTempBin |= lsbTemp;
  // data.totalTempBin <<= 4;
  // data.totalTempBin |= xlsbTemp;
}

void BMP280::readTempParam()
{
  data.dig_t1 = readSingleByte(adresses::dig_t1Adress1) << 8;
  data.dig_t1 |= readSingleByte(adresses::dig_t1Adress2);

  data.dig_t2 = readSingleByte(adresses::dig_t2Adress1) << 8;
  data.dig_t2 |= readSingleByte(adresses::dig_t2Adress2);

  data.dig_t3 = readSingleByte(adresses::dig_t3Adress1) << 8;
  data.dig_t3 |= readSingleByte(adresses::dig_t3Adress2);
}

void BMP280::readPressParam()
{
  data.dig_p1 = readSingleByte(adresses::dig_p1Adress1) << 8;
  data.dig_p1 |= readSingleByte(adresses::dig_p1Adress2);

  data.dig_p2 = readSingleByte(adresses::dig_p2Adress1) << 8;
  data.dig_p2 |= readSingleByte(adresses::dig_p2Adress2);

  data.dig_p3 = readSingleByte(adresses::dig_p3Adress1) << 8;
  data.dig_p3 |= readSingleByte(adresses::dig_p3Adress2);

  data.dig_p4 = readSingleByte(adresses::dig_p4Adress1) << 8;
  data.dig_p4 |= readSingleByte(adresses::dig_p4Adress2);

  data.dig_p5 = readSingleByte(adresses::dig_p5Adress1) << 8;
  data.dig_p5 |= readSingleByte(adresses::dig_p5Adress2);

  data.dig_p6 = readSingleByte(adresses::dig_p6Adress1) << 8;
  data.dig_p6 |= readSingleByte(adresses::dig_p6Adress2);

  data.dig_p7 = readSingleByte(adresses::dig_p7Adress1) << 8;
  data.dig_p7 |= readSingleByte(adresses::dig_p7Adress2);

  data.dig_p8 = readSingleByte(adresses::dig_p8Adress1) << 8;
  data.dig_p8 |= readSingleByte(adresses::dig_p8Adress2);

  data.dig_p9 = readSingleByte(adresses::dig_p9Adress1) << 8;
  data.dig_p9 |= readSingleByte(adresses::dig_p9Adress2);
}

void BMP280::readId()
{
  uint8_t id = readSingleByte(adresses::idAdress);
  if (id == 88)
  {
    data.id = id;
  }
  else
  {
    data.id = 255;//error
  }
  
}

void BMP280::setMode()
{
  //eerste 3 bits = temp, 2e 3 bits = pressure laatste 2 is mode.
  const uint8_t mode = 0b00100101; //temperature oversampeling = 1x, pressure oversampling = 1x, forced mode
  const uint8_t config = 0b00000000;
  writeSingleByte(adresses::ctrl_measAdress, mode);
  writeSingleByte(adresses::configAdress, config);
}

void BMP280::reset()
{
  writeSingleByte(adresses::resetAdress, 0xB6);
  hwlib::wait_ms(10);
}

void BMP280::calculateTemp()
{
  int32_t var1, var2, var3;
  var1 = ((((data.totalTempBin >> 3) - ((int32_t)data.dig_t1 << 1))) * ((int32_t)data.dig_t2)) >> 11;
  var2 = (((((data.totalTempBin >> 4) - ((int32_t)data.dig_t1)) * ((data.totalTempBin >> 4) - ((int32_t)data.dig_t1))) >> 12) *
          ((int32_t)data.dig_t3)) >>
         14;
  var3 = var1 + var2;
  data.realTemp = ((var3 * 5) + 128) >> 8;
  hwlib::cout << data.realTemp/100 << hwlib::endl;
}

BMP280::BMPData BMP280::returnData()
{
  return data;
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