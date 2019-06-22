#include "BMP280.hpp"

BMP280::BMP280(hwlib::i2c_bus &bus) : BoschBM(bus) {}

int16_t BMP280::readTemperature()
{
  setMode((uint8_t)BMP280::precision::oversampelingOneTime);
  readParam();
  readPTregisters();
  int16_t t = calculateTemp();
  return t;
}

uint32_t BMP280::readPressure()
{
  uint32_t p = calculatePress();
  return p;
}

void BMP280::readParam()
{
  if (!paramRead)
  {
    paramRead = 1;
    uint8_t buffer[24];
    burstRead((uint8_t)BMP280::reg::i2cRegister, (uint8_t)BMP280::reg::dig_t1Register1, buffer, 24);

    data.dig_t1 |= buffer[1];
    data.dig_t1 <<= 8;
    data.dig_t1 |= buffer[0];
    data.dig_t2 |= buffer[3];
    data.dig_t2 <<= 8;
    data.dig_t2 |= buffer[2];
    data.dig_t3 |= buffer[5];
    data.dig_t3 <<= 8;
    data.dig_t3 |= buffer[4];

    data.dig_p1 |= buffer[7];
    data.dig_p1 <<= 8;
    data.dig_p1 |= buffer[6];
    data.dig_p2 |= buffer[9];
    data.dig_p2 <<= 8;
    data.dig_p2 |= buffer[8];
    data.dig_p3 |= buffer[11];
    data.dig_p3 <<= 8;
    data.dig_p3 |= buffer[10];
    data.dig_p4 |= buffer[13];
    data.dig_p4 <<= 8;
    data.dig_p4 |= buffer[12];
    data.dig_p5 |= buffer[15];
    data.dig_p5 <<= 8;
    data.dig_p5 |= buffer[14];
    data.dig_p6 |= buffer[17];
    data.dig_p6 <<= 8;
    data.dig_p6 |= buffer[16];
    data.dig_p7 |= buffer[19];
    data.dig_p7 <<= 8;
    data.dig_p7 |= buffer[18];
    data.dig_p8 |= buffer[21];
    data.dig_p8 <<= 8;
    data.dig_p8 |= buffer[20];
    data.dig_p9 |= buffer[23];
    data.dig_p9 <<= 8;
    data.dig_p9 |= buffer[22];
  }
}

void BMP280::reset()
{
  writeSingleByte((uint8_t)BMP280::reg::i2cRegister, (uint8_t)BMP280::reg::resetRegister, 0xB6);
  hwlib::wait_ms(10);
}

uint8_t BMP280::readId()
{
  return readSingleByte((uint8_t)BMP280::reg::i2cRegister, (uint8_t)BMP280::reg::idRegister);
}

void BMP280::readPTregisters()
{
  uint8_t buffer[6];
  burstRead((uint8_t)BMP280::reg::i2cRegister, (uint8_t)BMP280::reg::pressureRegister1, buffer, 6);
  buffer[2] >>= 4;
  buffer[5] >>= 4;

  data.totalPressBin |= buffer[0];
  data.totalPressBin <<= 8;
  data.totalPressBin |= buffer[1];
  data.totalPressBin <<= 4;
  data.totalPressBin |= buffer[2];

  data.totalTempBin |= buffer[3];
  data.totalTempBin <<= 8;
  data.totalTempBin |= buffer[4];
  data.totalTempBin <<= 4;
  data.totalTempBin |= buffer[5];
}

void BMP280::setMode(uint8_t mode)
{
  const uint8_t config = 0b00000100; //pressure filter on
  writeSingleByte((uint8_t)BMP280::reg::i2cRegister, (uint8_t)BMP280::reg::ctrl_measRegister, mode);
  hwlib::wait_ms(10);
  writeSingleByte((uint8_t)BMP280::reg::i2cRegister, (uint8_t)BMP280::reg::configRegister, config);
  hwlib::wait_ms(10);
}

int16_t BMP280::calculateTemp()
{
  int32_t var1, var2;
  var1 = ((((data.totalTempBin >> 3) - ((int32_t)data.dig_t1 << 1))) * ((int32_t)data.dig_t2)) >> 11;
  var2 = (((((data.totalTempBin >> 4) - ((int32_t)data.dig_t1)) * ((data.totalTempBin >> 4) - ((int32_t)data.dig_t1))) >> 12) *
          ((int32_t)data.dig_t3)) >>
         14;
  data.t_fine = var1 + var2;
  data.totalTempBin = 0;
  return ((data.t_fine * 5 + 128) >> 8);
}

uint32_t BMP280::calculatePress()
{
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
    return 0;
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
  var1 = (((int32_t)data.dig_p9) * ((int32_t)(((p >> 3) * (p >> 3)) >> 13))) >> 12;
  var2 = (((int32_t)(p >> 2)) * ((int32_t)data.dig_p8)) >> 13;
  p = (uint32_t)((int32_t)p + ((var1 + var2 + data.dig_p7) >> 4));
  data.totalPressBin = 0;
  return p;
}

BMP280::BMPData BMP280::returnDataStruct()
{
  return data;
}
