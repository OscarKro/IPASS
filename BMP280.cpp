#include "BMP280.hpp"

BMP280::BMP280(hwlib::i2c_bus &bus) : BoschBM(bus) {}

//burst readout from the raw temp and pressure registers.
void BMP280::readPTregisters()
{
  uint8_t buffer[6];
  burstRead(adresses::i2cAdress,adresses::pressureAdress1, buffer ,6);
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

//function to read al the temperature trimming registers.
void BMP280::readTempParam()
{
  data.dig_t1 |= readSingleByte(adresses::i2cAdress, adresses::dig_t1Adress2);
  data.dig_t1 <<= 8;
  data.dig_t1 |= readSingleByte(adresses::i2cAdress, adresses::dig_t1Adress1);

  data.dig_t2 |= readSingleByte(adresses::i2cAdress, adresses::dig_t2Adress2);
  data.dig_t2 <<= 8;
  data.dig_t2 |= readSingleByte(adresses::i2cAdress, adresses::dig_t2Adress1);

  data.dig_t3 |= readSingleByte(adresses::i2cAdress, adresses::dig_t3Adress2);
  data.dig_t3 <<= 8;
  data.dig_t3 |= readSingleByte(adresses::i2cAdress, adresses::dig_t3Adress1);
}

//read the pressure trimming registers.
void BMP280::readPressParam()
{
  data.dig_p1 |= readSingleByte(adresses::i2cAdress, adresses::dig_p1Adress2);
  data.dig_p1 <<= 8;
  data.dig_p1 |= readSingleByte(adresses::i2cAdress, adresses::dig_p1Adress1);

  data.dig_p2 |= readSingleByte(adresses::i2cAdress, adresses::dig_p2Adress2);
  data.dig_p2 <<= 8;
  data.dig_p2 |= readSingleByte(adresses::i2cAdress, adresses::dig_p2Adress1);

  data.dig_p3 |= readSingleByte(adresses::i2cAdress, adresses::dig_p3Adress2);
  data.dig_p3 <<= 8;
  data.dig_p3 |= readSingleByte(adresses::i2cAdress, adresses::dig_p3Adress1);

  data.dig_p4 |= readSingleByte(adresses::i2cAdress, adresses::dig_p4Adress2);
  data.dig_p4 <<= 8;
  data.dig_p4 |= readSingleByte(adresses::i2cAdress, adresses::dig_p4Adress1);

  data.dig_p5 |= readSingleByte(adresses::i2cAdress, adresses::dig_p5Adress2);
  data.dig_p5 <<= 8;
  data.dig_p5 |= readSingleByte(adresses::i2cAdress, adresses::dig_p5Adress1);

  data.dig_p6 |= readSingleByte(adresses::i2cAdress, adresses::dig_p6Adress2);
  data.dig_p6 <<= 8;
  data.dig_p6 |= readSingleByte(adresses::i2cAdress, adresses::dig_p6Adress1);

  data.dig_p7 |= readSingleByte(adresses::i2cAdress, adresses::dig_p7Adress2);
  data.dig_p7 <<= 8;
  data.dig_p7 |= readSingleByte(adresses::i2cAdress, adresses::dig_p7Adress1);

  data.dig_p8 |= readSingleByte(adresses::i2cAdress, adresses::dig_p8Adress2);
  data.dig_p8 <<= 8;
  data.dig_p8 |= readSingleByte(adresses::i2cAdress, adresses::dig_p8Adress1);

  data.dig_p9 |= readSingleByte(adresses::i2cAdress, adresses::dig_p9Adress2);
  data.dig_p9 <<= 8;
  data.dig_p9 |= readSingleByte(adresses::i2cAdress, adresses::dig_p9Adress1);
}

//read the id of the chip (shoud be 88) return true if succeeded.
bool BMP280::readId()
{
  ;
  if (readSingleByte(adresses::i2cAdress, adresses::idAdress) == 0x58)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

//set the oversampling, measurement and filter modes on the chip.
void BMP280::setMode(uint8_t mode)
{
  const uint8_t config = 0b00000100; //pressure filter on
  writeSingleByte(adresses::i2cAdress, adresses::ctrl_measAdress, mode);
  hwlib::wait_ms(10);
  writeSingleByte(adresses::i2cAdress, adresses::configAdress, config);
  hwlib::wait_ms(10);
}

 //reset the chip and all the registers.
void BMP280::reset()
{
  writeSingleByte(adresses::i2cAdress, adresses::resetAdress, 0xB6);
  hwlib::wait_ms(10);
}

//calculate the real temperature in C.
void BMP280::calculateTemp()
{
  int32_t var1, var2;
  var1 = ((((data.totalTempBin >> 3) - ((int32_t)data.dig_t1 << 1))) * ((int32_t)data.dig_t2)) >> 11;
  var2 = (((((data.totalTempBin >> 4) - ((int32_t)data.dig_t1)) * ((data.totalTempBin >> 4) - ((int32_t)data.dig_t1))) >> 12) *
          ((int32_t)data.dig_t3)) >>
         14;
  data.t_fine = var1 + var2;
  data.realTemp = ((data.t_fine * 5 + 128) >> 8) / 100;
  data.totalTempBin = 0;
}

//calculate the real pressure in hPa.
void BMP280::calculatePress()
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
  var1 = (((int32_t)data.dig_p9) * ((int32_t)(((p >> 3) * (p >> 3)) >> 13))) >> 12;
  var2 = (((int32_t)(p >> 2)) * ((int32_t)data.dig_p8)) >> 13;
  p = (uint32_t)((int32_t)p + ((var1 + var2 + data.dig_p7) >> 4));
  data.realPress = p / 100;
  data.totalPressBin = 0;
}

//return the struct with all the data.
BMP280::BMPData BMP280::returnDataStruct()
{
  return data;
}

//return the struct with the possible resolution modes
BMP280::precisionMode BMP280::returnModeStruct()
{
  return mode;
}