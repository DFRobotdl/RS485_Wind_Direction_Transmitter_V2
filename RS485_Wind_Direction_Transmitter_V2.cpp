#include "RS485_Wind_Direction_Transmitter_V2.h"

RS485_Wind_Direction_Transmitter_V2::RS485_Wind_Direction_Transmitter_V2(HardwareSerial *hardSerial)
{
  hardSerial->begin(9600);
  _ser = hardSerial;
}

#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
RS485_Wind_Direction_Transmitter_V2::RS485_Wind_Direction_Transmitter_V2(SoftwareSerial *softSerial)
{
  softSerial->begin(9600);
  _ser = softSerial;
}
#elif defined(ESP32)
RS485_Wind_Direction_Transmitter_V2::RS485_Wind_Direction_Transmitter_V2(HardwareSerial *hardSerial, uint8_t rx, uint8_t tx)
{
  hardSerial->begin(9600, SERIAL_8N1, rx, tx);
  _ser = hardSerial;
}
#endif

bool RS485_Wind_Direction_Transmitter_V2::begin()
{
  //sendSequence = 0;

  if (NULL == _ser) {
    return false;
  }
  return true;
}

size_t RS485_Wind_Direction_Transmitter_V2::readN(uint8_t *buf, size_t len)
{
  size_t offset = 0, left = len;
  int16_t Tineout = 1500;
  uint8_t  *buffer = buf;
  long curr = millis();
  while (left) {
    if (_ser->available()) {
      buffer[offset] = _ser->read();
      offset++;
      left--;
    }
    if (millis() - curr > Tineout) {
      break;
    }
  }
  return offset;
}


uint16_t RS485_Wind_Direction_Transmitter_V2::CRC16_2(uint8_t *buf, int16_t len)
{
  uint16_t crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (uint16_t)buf[pos];
    for (int i = 8; i != 0; i--)
    {
      if ((crc & 0x0001) != 0)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }

  crc = ((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8);
  return crc;
}

void RS485_Wind_Direction_Transmitter_V2::addedCRC(uint8_t *buf, int len)
{
  uint16_t crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (uint16_t)buf[pos];
    for (int i = 8; i != 0; i--)
    {
      if ((crc & 0x0001) != 0)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  buf[len] = crc % 0x100;
  buf[len + 1] = crc / 0x100;
}

bool RS485_Wind_Direction_Transmitter_V2::readHoldingRegister(uint8_t Address, uint16_t reg, uint8_t *data)
{
  uint8_t buf[7] = {0};       //Store the original data packet returned by the sensor  
  uint8_t COM[8] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};   //Read register command 
  long curr = millis();    //Determine timeout 
  long curr1 = curr;       //Determine timeout 
  uint8_t ch = 0;          //Store serial data temporarily 
  COM[0] = Address;        //Refer to the communication protocol to add the complete command packet 
  COM[2] = (reg / 0x100) % 0x100;
  COM[3] = reg % 0x100;

  addedCRC(COM , 6);   //Add CRC_16 to the command for reading wind direction 
  _ser->write(COM, 8);  //Transmit read command 

  while (1) {
    if (millis() - curr > 1000) {
      return false;    //Timeout if correct data is not read in 1000ms, and return —1 
    }

    if (millis() - curr1 > 100) {   //Re-transmit command for reading wind direction if the return command is not received in 100ms after sending last wind direction reading command. 
      _ser->write(COM, 8);  //Transmit reading command 
      curr1 = millis();
    }

    if (readN(&ch, 1) == 1) {
      if (ch == Address) {          //Read and determine packet header 
        buf[0] = ch;
        if (readN(&ch, 1) == 1) {
          if (ch == 0x03) {         //Read and determine packet header 
            buf[1] = ch;
            if (readN(&ch, 1) == 1) {
              if (ch == 0x02) {       //Read and determine packet header 
                buf[2] = ch;
                if (readN(&buf[3], 4) == 4) {
                  //DBUG(buf, 8);
                  if (CRC16_2(buf, 5) == (buf[5] * 256 + buf[6])) {  //Check data packet 
                    data[0] = buf[3];
                    data[1] = buf[4];
                    return true;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

bool RS485_Wind_Direction_Transmitter_V2::writeHoldingRegister(uint8_t Address, uint16_t reg, uint16_t parameter)
{
  uint8_t COM[11] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00};  //Write register command 
  uint8_t buf[8] = {0};   //Store data returned by the sensor 
  long curr = millis();   //Determine timeout 
  long curr1 = curr;      //Determine timeout
  uint8_t ch = 0;         //Store serial data temporarily 
  COM[0] = Address;                    //Refer to the communication protocol to add complete command packet 
  COM[2] = (reg / 0x100) % 0x100;
  COM[3] = reg % 0x100;
  COM[7] = (parameter / 0x100) % 0x100;
  COM[8] = parameter % 0x100;
  addedCRC(COM , 9);            //Add CRC_16 check to command packet 
  _ser->write(COM, 11);  //Transmit command for reading wind direction 

  while (1) {
    if (millis() - curr > 1000) {
      return false;
    }

    if (millis() - curr1 > 100) {
      _ser->write(COM, 11);  //Transmit command for reading wind direction
      curr1 = millis();
    }

    if (readN(&ch, 1) == 1) {
      if (ch == Address) {
        buf[0] = ch;
        if (readN(&ch, 1) == 1) {
          if (ch == 0x10) {
            buf[1] = ch;
            if (readN(&ch, 1) == 1) {
              if (ch == COM[2]) {
                buf[2] = ch;
                if (readN(&ch, 1) == 1) {
                  if (ch == COM[3]) {
                    buf[3] = ch;
                    if (readN(&buf[4], 4) == 4) {
                      if (CRC16_2(buf, 6) == (buf[6] * 256 + buf[7])) {  //Check data packet 
                        //DBUG(buf, 8);
                        return true;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

uint8_t RS485_Wind_Direction_Transmitter_V2::GetSlaveAddress()
{
  uint8_t data[2] = {0};
  readHoldingRegister(Adderss, 0x1000, data);
  return data[1];
}

uint8_t RS485_Wind_Direction_Transmitter_V2::GetWindDirection(uint8_t Address)
{
  uint8_t data[2] = {0};
  readHoldingRegister(Adderss, 0x0001, data);
  return data[0] * 0x100 + data[1];
}

float RS485_Wind_Direction_Transmitter_V2::GetWindAngle(uint8_t Adderss)
{
  uint8_t data[2] = {0};
  readHoldingRegister(Adderss, 0x0000, data);
  return (data[0] * 0x100 + data[1]) / 10.0;
}

bool RS485_Wind_Direction_Transmitter_V2::SetSlaveAddress(uint8_t Address1, uint8_t Address2)
{
  return writeHoldingRegister(Address1, 0x1000, Address2);
}