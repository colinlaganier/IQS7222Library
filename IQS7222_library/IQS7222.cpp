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

/**
  * @name checkReset
  * @brief  A method which checks if the device has reset and returns the reset status.
  * @param  None.
  * @retval Returns true if a reset has occurred, false if no reset has occurred.
  * @notes  If a reset has occurred the device settings should be reloaded using the begin function.
  *     After new device settings have been reloaded the acknowledge reset function can be used
  *     to clear the reset flag.
  */
bool IQS7222::checkReset(bool stopOrRestart)
{
    uint8_t transferBytes[1]; // A temporary array to hold the byte to be transferred.
    // Read the System Flags from the IQS7222.
    readRandomBytes(SYS_FLAGS, 1, transferBytes, stopOrRestart);
    transferBytes[0] &= SHOW_RESET_BIT;
    // Return the reset status.
    if (transferBytes[0] != 0)
        return true;
    else
        return false;
}

/**
  * @name acknowledgeReset
  * @brief  A method which clears the Show Reset bit by writing it to a 0.
  * @param  None.
  * @retval None.
  * @notes  If a reset has occurred the device settings should be reloaded using the begin function.
  *     After new device settings have been reloaded this method should be used to clear the
  *     reset bit.
  */
void IQS7222::acknowledgeReset(bool stopOrRestart)
{
    uint8_t transferBytes[2]; // A temporary array to hold the bytes to be transferred.
    readRandomBytes(CONTROL_SETTING, 2, transferBytes, RESTART);
    // Write the Ack Reset bit to 1 to clear the Show Reset Flag.
    transferBytes[0] |= ACK_RESET_BIT;
    // Write the new byte to the System Flags address.
    writeRandomBytes(CONTROL_SETTING, 2, transferBytes, stopOrRestart);
}

/**
  * @name   autoTune
  * @brief  A method which sets the REDO_ATI_BIT in order to force the IQS7222 device to run the
  *         Automatic Tuning Implementation (ATI) routine.
  * @param  None.
  * @retval None.
  * @notes  To force ATI, bit 3 in CONTROL_SETTING is set.
  */
void IQS7222::autoTune(bool stopOrRestart)
{
    uint8_t transferByte[1]; // Array to store the bytes transferred.

    readRandomBytes(CONTROL_SETTING, 1, transferByte, RESTART);
    // Mask the settings with the REDO_ATI_BIT.
    transferByte[0] |= REDO_ATI_BIT;  // This is the bit required to start an ATI routine.
    // Write the new byte to the required device.
    writeRandomBytes(CONTROL_SETTING, 1, transferByte, stopOrRestart);
}

/**
  * @name   softReset
  * @brief  A method which sets the DO_RESET_BIT in order to force the IQS7222 device to reset device.
  * @param  None.
  * @retval None.
  * @notes  To force reset, bit 1 in CONTROL_SETTING is set.
  */
void IQS7222::softReset(bool stopOrRestart)
{
    uint8_t transferByte[1]; // Array to store the bytes transferred.

    readRandomBytes(CONTROL_SETTING, 1, transferByte, RESTART);
    // Mask the settings with the REDO_ATI_BIT.
    transferByte[0] |= REDO_ATI_BIT;  // This is the bit required to start an ATI routine.
    // Write the new byte to the required device.
    writeRandomBytes(CONTROL_SETTING, 1, transferByte, stopOrRestart);
}

/**
  * @name   printCounts
  * @brief  A method which reads the current channel counts and prints them in the Serial Monitor
  * @param  None.
  * @retval None.
  * @notes  None.
  */
void IQS7222::printCounts(bool stopOrRestart)
{
    uint8_t transferByte[10]; // Array to store the bytes transferred.
    readRandomBytes(CH0_COUNTS, 10, transferByte, STOP);
    for (const uint8_t &count : transferByte)
    {
        Serial.print(count);
        Serial.print(", ");
    }
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

void IQS7222::initialSetup(bool stopOrRestart)
{

}

