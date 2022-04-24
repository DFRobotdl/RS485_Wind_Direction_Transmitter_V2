# RS485 Wind Direction Transmitter V2

The wind direction sensor is a professional meteorological instrument used to measure horizontal wind direction. It integrates a hall sensor inside. The shell and weather vane is made of aluminum alloy material and uses special mold precision die casting process, featuring small dimension tolerance and high surface accuracy. The sensor internal circuit is protected, which makes the sensor have the properties of high strength, weather resistance, anti-corrosion, and waterproof. The cable connector adopts a corrosion-resistant military plug that ensures the long service life of the product.

A low-inertia wind vane is used to sense the wind direction. When the wind direction changes, the tail wing rotates to drive the axle magnet to rotate through the shaft, thereby obtaining accurate wind direction information. It can detect the 16 azimuths' natural wind directions. This high-cost performance sensor can be widely used in meteorology, ocean, environment, airports, ports, laboratories, industry, agriculture, and transportation.

## Table of Contents

* [Installation](#installation)
* [Methods](#methods)
* [History](#history)
* [Credits](#credits)

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
     /**
       @brief constructor

    */
    RS485_Wind_Direction_Transmitter_V2();
    ~RS485_Wind_Direction_Transmitter_V2();

    /**
       @brief init

       @param myhardSerial serial interface
    */
    void begin(HardwareSerial &myhardSerial);

    /**
       @brief init

       @param mysoftSerial serial interface
    */
    void begin(SoftwareSerial &mysoftSerial);

    /**
       @brief Read wind direction (16 wind directions)

       @param Address Modbus Slave address 
       @return Wind direction (0-16)
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
       @brief Read wind direction angle (360 degrees)

       @param Adderss Modbus Slave address 
       @return float Wind direction angle (0~360 degrees)
    */
    float GetWindAngle(uint8_t Adderss);

    /**
       @brief Revise wind direction sensor Modbus slave address. It takes effect right after being modified. 

       @param Address1 Current Modbus slave address.(0x00 is boardcast address, which all slaves will process broadcast packets, but will not answer.)
       @param Address2 The address you want to change to 
       @return true Succeeded
       @return false Failed
    */
    bool SetSlaveAddress(uint8_t Address1, uint8_t Address2);

    /**
      @brief Read wind direction sensor Modbus salve address 

      @return Modbus Slave address(0-255)
    */
    uint8_t GetSlaveAddress();
```

## Compatibility

| MCU           | Work Well | Work Wrong | Untested | Remarks |
| ------------- | :-------: | :--------: | :------: | ------- |
| Arduino uno   |     √     |            |          |         |
| Mega2560      |     √     |            |          |         |
| Leonardo      |           |            |      √   |         |
| ESP32         |     √     |            |          |         |
|               |           |            |          |         |


## History

- data 2022-04-17
- version V1.0


## Credits

Written by huyujie(yujie.hu@dfrobot.com), 2020. (Welcome to our [website](https://www.dfrobot.com/))

# DFRobot_mmWave_Radar
