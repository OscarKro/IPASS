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
  uint8_t msbPress = transaction.read_byte();
  uint8_t lsbPress = transaction.read_byte();
  uint8_t xlsbPress = transaction.read_byte();
  uint8_t msbTemp = transaction.read_byte();
  uint8_t lsbTemp = transaction.read_byte();
  uint8_t xlsbTemp = transaction.read_byte();
  xlsbPress >>= 4;
  xlsbTemp >>= 4;

  data.totalPressBin |= msbPress;
  data.totalPressBin <<= 8;
  data.totalPressBin |= lsbPress;
  data.totalPressBin <<= 4;
  data.totalPressBin |= xlsbPress;

  data.totalTempBin |= msbTemp;
  data.totalTempBin <<= 8;
  data.totalTempBin |= lsbTemp;
  data.totalTempBin <<= 4;
  data.totalTempBin |= xlsbTemp;
}

void BMP280::readTempParam()
{
  data.dig_t1 = readSingleByte(adresses::dig_t1Adress2);
  data.dig_t1 <<= 8;
  data.dig_t1 |= readSingleByte(adresses::dig_t1Adress1);

  data.dig_t2 = readSingleByte(adresses::dig_t2Adress2);
  data.dig_t2 <<= 8;
  data.dig_t2 |= readSingleByte(adresses::dig_t2Adress1);

  data.dig_t3 = readSingleByte(adresses::dig_t3Adress2);
  data.dig_t3 <<= 8;
  data.dig_t3 |= readSingleByte(adresses::dig_t3Adress1);
}

void BMP280::readPressParam()
{
  data.dig_p1 = readSingleByte(adresses::dig_p1Adress2);
  data.dig_p1 <<= 8;
  data.dig_p1 |= readSingleByte(adresses::dig_p1Adress1);

  data.dig_p2 = readSingleByte(adresses::dig_p2Adress2);
  data.dig_p2 <<= 8;
  data.dig_p2 |= readSingleByte(adresses::dig_p2Adress1);

  data.dig_p3 = readSingleByte(adresses::dig_p3Adress2);
  data.dig_p3 <<= 8;
  data.dig_p3 |= readSingleByte(adresses::dig_p3Adress1);

  data.dig_p4 = readSingleByte(adresses::dig_p4Adress2);
  data.dig_p4 <<= 8;
  data.dig_p4 |= readSingleByte(adresses::dig_p4Adress1);

  data.dig_p5 = readSingleByte(adresses::dig_p5Adress2);
  data.dig_p5 <<= 8;
  data.dig_p5 |= readSingleByte(adresses::dig_p5Adress1);

  data.dig_p6 = readSingleByte(adresses::dig_p6Adress2);
  data.dig_p6 <<= 8;
  data.dig_p6 |= readSingleByte(adresses::dig_p6Adress1);

  data.dig_p7 = readSingleByte(adresses::dig_p7Adress2);
  data.dig_p7 <<= 8;
  data.dig_p7 |= readSingleByte(adresses::dig_p7Adress1);

  data.dig_p8 = readSingleByte(adresses::dig_p8Adress2);
  data.dig_p8 <<= 8;
  data.dig_p8 |= readSingleByte(adresses::dig_p8Adress1);

  data.dig_p9 = readSingleByte(adresses::dig_p9Adress2);
  data.dig_p9 <<= 8;
  data.dig_p9 |= readSingleByte(adresses::dig_p9Adress1);
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
    data.id = 255; //error
  }
}

void BMP280::setMode()
{
  const uint8_t mode = 0b00100101;//pressure and temperature oversampling set to 1*, mode = forced mode.
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
  //page 46 datasheet (32 bit architecture)
  int32_t var1, var2;
  var1 = ((((data.totalTempBin >> 3) - ((int32_t)data.dig_t1 << 1))) * ((int32_t)data.dig_t2)) >> 11;
  var2 = (((((data.totalTempBin >> 4) - ((int32_t)data.dig_t1)) * ((data.totalTempBin >> 4) - ((int32_t)data.dig_t1))) >> 12) *
          ((int32_t)data.dig_t3)) >>
         14;
  data.t_fine = var1 + var2;
  data.realTemp = ((data.t_fine * 5 + 128) >> 8)/100;
}

void BMP280::calculatePress()
{
  //page 46 datasheet (32 bit architecture)
  int32_t var1, var2;
  uint32_t p;
  var1 = (data.t_fine >> 1) - (int32_t)64000;
  var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)data.dig_p6);
  var2 = var2 + ((var1 * ((int32_t)data.dig_p5)) << 1);
  var2 = (var2 >> 2) + (((int32_t)data.dig_p4) << 16);
  var1 = (((data.dig_p3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t)data.dig_p2) * var1) >> 1)) >> 18;
  var1 = ((((32768 + var1)) * ((int32_t)data.dig_p1)) >> 15);
  if (var1 == 0)
  {
    data.realPress = 0;
    return;
  }
  p = (((uint32_t)(((int32_t)1048576) - data.totalPressBin) - (var2 >> 12))) * 3125;
  if (p < 0x80000000)
  {
    p = (p << 1) / ((uint32_t)var1);
  }
  else
  {
    p = (p / (uint32_t)var1) * 2;
  }
  var1 = (((int32_t)data.dig_p9) * ((int32_t)(((p>>3) * (p >>3))>>13))) >>12;
  var2 = (((int32_t)(p>>2)) * ((int32_t)data.dig_p8)) >> 13;
  p = (uint32_t)((int32_t)p + ((var1 + var2 + data.dig_p7) >> 4));
  data.realPress = p/100;
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