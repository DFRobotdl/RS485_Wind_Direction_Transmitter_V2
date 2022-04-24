#include "RS485_Wind_Direction_Transmitter_V2.h"


/**
   @brief RS485_Wind_Direction_Transmitter_V2 constructor
   @param serial - serial ports for communication, supporting hard and soft serial ports
   @param rx - UART Pin for receiving data 
   @param tx - UART Pin for transmitting data 
*/
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)   // Use softserial 
SoftwareSerial softSerial(/*rx =*/2, /*tx =*/3);
RS485_Wind_Direction_Transmitter_V2 windDirection(/*softSerial =*/&softSerial);
#elif defined(ESP32)   // Use the hardserial of remappable pin: Serial1
RS485_Wind_Direction_Transmitter_V2 windDirection(/*hardSerial =*/&Serial1, /*rx =*/D3, /*tx =*/D2);
#else   // Use hardserial: Serial1
RS485_Wind_Direction_Transmitter_V2 windDirection(/*hardSerial =*/&Serial1);
#endif

uint8_t Address = 0x02;

const char* Orientation[17] = {
  "north", "north by northeast", "northeast", "east by northeast", "east", "east by southeast", "southeast", "south by southeast", "south",
  "south by southwest", "southwest", "west by southwest", "west", "west by northwest", "northwest", "north by northwest", "north"
};

void setup()
{
  Serial.begin(115200);

  // Init the sensor
  while ( !( windDirection.begin() ) ) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");
//Revise Modbus slave address. You can use broadcast address 0x00 to revise if the current address is forgotten. 
//  windDirection.SetSlaveAddress(/*Current modbus slave address*/0x00, /*Revised address*/0x02);
//Get modbus slave address 
//  Address = windDirection.GetSlaveAddress();
}

void loop()
{
  //Get 16 wind directions 
  int Direction = windDirection.GetWindDirection(/*modbus slave address*/Address);
  //Get 360° wind direction angle 
  float Angle = windDirection.GetWindAngle(/*modbus slave address*/Address);
  Serial.println(Orientation[Direction]);
  Serial.print(Angle); Serial.println("°");
  Serial.println();
  delay(1000);
}