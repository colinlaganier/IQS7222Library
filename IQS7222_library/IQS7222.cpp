/**
  **********************************************************************************
  * @file     IQS7222.cpp
  * @author   Colin Laganier
  * @version  V0.1
  * @date     2021-08-10
  * @brief   This file contains the constructors and method for an IQS7222 Arduino library.
  *          The goal of the library is to provide the functionalities for touch sensing
  *			 to prototypes. Library derived from Azoteq's IQS266 library example.
  **********************************************************************************
  * @attention  Requires standard Arduino Libraries: Arduino.h, Wire.h.
  */

  // Include Files
#include "IQS7222.h"

/**************************************************************************************************************/
/*                                                CONSTRUCTORS                                                */
/**************************************************************************************************************/
IQS7222::IQS7222() {
}

/**************************************************************************************************************/
/*                                              PUBLIC METHODS                                                */
/**************************************************************************************************************/

/**
  * @name   begin
  * @brief  Method to initialize the IQS7222 device with the device address and ready pin specified by the user.
  * @param  deviceAddress -> The address of the IQS7222 device.
  *         readyPin      -> The Arduino pin connected to the ready pin of the IQS7222 device.
  * @retval Returns true if communication has been successfully established, returns false if not.
  * @notes  Receiving a true return value does not mean that initialization was successful.
  *         Receiving a true return value only means that the IQS device responded to the request for communication.
  *         Receiving a false return value means that initialization did not take place at all.
  *         If communication is successfully established then it is unlikely than initialization will fail.
  */
bool IQS7222::begin(uint8_t deviceAddressIn, uint8_t readyPinIn)
{
    _deviceAddress = deviceAddressIn;
    _readyPin = readyPinIn;

    // Initialize I2C communication
    Wire.begin();
    Wire.setClock(400000);

    // Request communication and run ATI routine.
    bool response = false;
    response = requestComms();
    if (response)
    {
        acknowledgeReset(RESTART);
        //autoTune(STOP);
    }
    return response;
}

/**
  * @name   beginHeadless
  * @brief  Method to initialize the IQS7222 device with the device address without a ready pin.
  * @param  deviceAddress -> The address of the IQS7222 device.
  * @retval Returns true if communication has been successfully established, returns false if not.
  * @notes  Not implemented yet
  */
bool IQS7222::beginHeadless(uint8_t deviceAddressIn)
{
    _deviceAddress = deviceAddressIn;

    // Initialize I2C communication
    Wire.begin();
    Wire.setClock(400000);
}

/**
  * @name   requestComms
  * @brief  Method to request communication by briefly pulling the READY pin of the IQS7222 LOW and waiting a response,
  *         the response by the IQS7222 is done by pulling the READY pin LOW again once the MCU has released it.
  *         If the IQS7222 device does not respond in 10ms the ready is pulled low again, this repeats 10 times before a false value is returned.
  *         If no response is received within 100ms the method returns false else true.
  * @param  None.
  * @retval Boolean: True if a response is received within 100ms, false if not.
  * @notes  Use this function when the master must initiate communication.
  */
bool IQS7222::requestComms(void)
{
    bool response = false;        // The return value. Is set to true if a response is received within 100ms.
    uint16_t notReadyCount = 0;  // Increments every time the loop executes to keep track of how long the request is going on.

    // Pull the ready bus LOW to let the IQS7222 device know you want to communicate.
    toggleReady();

    // Wait for the IQS7222 to respond by pulling the ready bus LOW. Redo request every 10ms, return false after 100ms.
    while (digitalRead(_readyPin))
    {
        notReadyCount++;
        delayMicroseconds(100);

        if ((notReadyCount % 1000) == 0)
            return response;
        if ((notReadyCount % 100) == 0)
            toggleReady();
    }
    response = true;
    return response;
}


/**************************************************************************************************************/
/*                                              PRIVATE METHODS                                               */
/**************************************************************************************************************/

/**
  * @name   toggleReady
  * @brief  A method which toggles the READY pin of an IQS7222 device.
  * @param  None.
  * @retval None.
  * @notes  Change pin to OUTPUT, pull LOW, delay, pull HIGH, change pin back to INPUT.
  */
void IQS7222::toggleReady(void)
{
    pinMode(_readyPin, OUTPUT);
    digitalWrite(_readyPin, LOW);
    delay(5);
    digitalWrite(_readyPin, HIGH);
    pinMode(_readyPin, INPUT);
}

void IQS7222::readRandomBytes(uint16_t memoryAddress, uint8_t numBytes, uint8_t bytesArray[], bool stopOrRestart)
{
    uint8_t i = 0;  // A simple counter to assist with loading bytes into the user supplied array.

    // Select the device with the address of "_deviceAddress" and start communication.
    Wire.beginTransmission(_deviceAddress);
    
    // Verifies if 8bit or 16bit address
    if (memoryAddress > 0xFF)
    {
        // Send a byte asking for the "memoryAddress" register 
        Wire.write(memoryAddress);
    }
    else
    {
        // Send two bytes asking for the "memoryAddress" register in little endian byte order
        Wire.write(memoryAddress & 0xFF);
        Wire.write(memoryAddress & 0xFF00 >> 8);
    }

    // Complete the selection and communication initialization.
    Wire.endTransmission(RESTART);  // Restart transmission for reading that follows.

    // Request "numBytes" bytes from the device which has address "_deviceAddress"
    do
    {
        Wire.requestFrom(_deviceAddress, numBytes, stopOrRestart);
    } while (Wire.available() == 0);  // Wait for response, this sometimes takes a few attempts

    // Load the received bytes into the array until there are no more 
    while (Wire.available())
    {
        // Load the received bytes into the user supplied array
        bytesArray[i] = Wire.read();
        i++;
    }
}

void IQS7222::writeRandomBytes(uint16_t memoryAddress, uint8_t numBytes, uint8_t bytesArray[], bool stopOrRestart)
{
    // Select the device with the address of "_deviceAddress" and start communication.
    Wire.beginTransmission(_deviceAddress);
    // Verifies if 8bit or 16bit address
    if (memoryAddress > 0xFF)
    {
        // Send a byte asking for the "memoryAddress" register 
        Wire.write(memoryAddress);
    }
    else
    {
        // Send two bytes asking for the "memoryAddress" register in little endian byte order.
        Wire.write(memoryAddress & 0xFF);
        Wire.write(memoryAddress & 0xFF00 >> 8);
    }
    // Write the bytes as specified in the array which "arrayAddress" pointer points to.
    for (int i = 0; i < numBytes; i++)
    {
        Wire.write(bytesArray[i]);
    }
    // End the transmission, user decides to STOP or RESTART.
    Wire.endTransmission(stopOrRestart);
}
