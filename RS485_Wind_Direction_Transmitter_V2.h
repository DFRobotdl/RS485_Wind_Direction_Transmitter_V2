#ifndef __RS485_Wind_Direction_Transmitter_V2_H__
#define __RS485_Wind_Direction_Transmitter_V2_H__

#include "Arduino.h"
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
#include <SoftwareSerial.h>
#endif

class RS485_Wind_Direction_Transmitter_V2
{
  public:
    /**
      @brief DFRobot_URM13_RTU constructor
      @param serial - serial ports for communication, supporting hard and soft serial ports
      @param rx - UART Pin for receiving data 
      @param tx - UART Pin for transmitting data 
    */
    RS485_Wind_Direction_Transmitter_V2(HardwareSerial *hardSerial);
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
    RS485_Wind_Direction_Transmitter_V2(SoftwareSerial *softSerial);
#elif defined(ESP32)
    RS485_Wind_Direction_Transmitter_V2(HardwareSerial *hardSerial, uint8_t rx, uint8_t tx);
#endif

    /**
       @brief Init

       @return true  Succeeded 
       @return false Failed 
    */
    bool begin();

    /**
       @brief Read 16 wind directions 

       @param Address Modbus Slave address 
       @return Wind direction(0-16)
      Return the value to the direction correspondence table
      -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
      |   value   |   0   |     1     |     2     |     3     |  4   |     5     |     6     |     7     |   8   |     9     |     10    |     11    |  12  |    13     |     14    |     15    |     16    |
      |-----------|-------|-----------|-----------|-----------|------|-----------|-----------|-----------|-------|-----------|-----------|-----------|------|-----------|-----------|-----------|-----------|
      | Direction | North | Northeast | Northeast | Northeast | East | Southeast | Southeast | Southeast | South | Southwest | Southwest | Southwest | West | Northwest | Northwest | Northwest |    North  |
      |           |       | by north  |           |  by east  |      |  by east  |           | by south  |       | by south  |           |  by west  |      |  by west  |           | by north  |           |
      -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    */
    uint8_t GetWindDirection(uint8_t Address);

    /**
       @brief Read wind direction angle(360 degrees) 

       @param Adderss Modbus Slave address 
       @return float Wind direction angle(0~360 degrees)
    */
    float GetWindAngle(uint8_t Adderss);

    /**
       @brief Revise wind drection sensor Modbus slave address. It takes effect right after being changed. 

       @param Address1 Current Modbus slave address(0x00 is broadcast address, which all slaves will process broadcast packets but will not answer)
       @param Address2 The address you want to change to 
       @return true Succeeded 
       @return false Failed 
    */
    bool SetSlaveAddress(uint8_t Address1, uint8_t Address2);

    /**
      @brief Read wind direction sensor Modbus slave address 

      @return Modbus slave address(0-255)
    */
    uint8_t GetSlaveAddress();

  private:

    Stream *_ser;
    // uart *_uartSerial;

    size_t readN(uint8_t *buf, size_t len);

    /**
       @brief Calculate CRC16_2 check value 

       @param buf Data packet of the check value to be calculated 
       @param len Length of data to be checked 
       @return uint16_t Return a 16bit check result 
    */
    uint16_t CRC16_2(uint8_t *buf, int16_t len);

    /**
       @brief Add CRC_16 check to the end of data packet 

       @param buf Data packet that needs to add check value
       @param len Length of data that needs to add check 
    */
    void addedCRC(uint8_t *buf, int len);

    /**
       @brief Read single Modbus register 

       @param Address Modbus Slave address 
       @param reg Regsiter address 
       @param data Store the data to be read, a register store data of 2*uint8_t
       @return true Succeeded 
       @return false Failed 
    */
    bool readHoldingRegister(uint8_t Address, uint16_t reg, uint8_t *data);

    /**
       @brief Write single Modbus register 

       @param Address Modbus slave address 
       @param reg Register address 
       @param parameter Data to be written 
       @return true Succeeded 
       @return false Failed 
    */
    bool writeHoldingRegister(uint8_t Address, uint16_t reg, uint16_t parameter);

};

#endif