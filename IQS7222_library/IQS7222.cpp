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
#include "IQS7222C_arduino_init.h"

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
        Serial.println("Initial Setup Begin");
        acknowledgeReset(RESTART);
        initialSetup(STOP);
        Serial.println("Initial Setup Complete");
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
  * @param  stopOrRestart -> A boolean which specifies whether the communication window should remain open or be closed after transfer.
  *                           False keeps it open, true closes it. Use the STOP and RESTART definitions. 
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
  * @param  stopOrRestart -> A boolean which specifies whether the communication window should remain open or be closed after transfer.
  *                           False keeps it open, true closes it. Use the STOP and RESTART definitions.
  * @retval None.
  * @notes  To force ATI, bit 3 in CONTROL_SETTING is set.
  */
void IQS7222::autoTune(bool stopOrRestart)
{
    uint8_t transferBytes[1]; // Array to store the bytes transferred.

    readRandomBytes(CONTROL_SETTING, 1, transferBytes, RESTART);
    // Mask the settings with the REDO_ATI_BIT.
    transferBytes[0] |= REDO_ATI_BIT;  // This is the bit required to start an ATI routine.
    // Write the new byte to the required device.
    writeRandomBytes(CONTROL_SETTING, 1, transferBytes, stopOrRestart);
}

/**
  * @name   softReset
  * @brief  A method which sets the DO_RESET_BIT in order to force the IQS7222 device to reset device.
  * @param  stopOrRestart -> A boolean which specifies whether the communication window should remain open or be closed after transfer.
  *                           False keeps it open, true closes it. Use the STOP and RESTART definitions.
  * @retval None.
  * @notes  To force reset, bit 1 in CONTROL_SETTING is set.
  */
void IQS7222::softReset(bool stopOrRestart)
{
    uint8_t transferBytes[1]; // Array to store the bytes transferred.

    readRandomBytes(CONTROL_SETTING, 1, transferBytes, RESTART);
    // Mask the settings with the REDO_ATI_BIT.
    transferBytes[0] |= REDO_ATI_BIT;  // This is the bit required to start an ATI routine.
    // Write the new byte to the required device.
    writeRandomBytes(CONTROL_SETTING, 1, transferBytes, stopOrRestart);
}

/**
  * @name   printCounts
  * @brief  A method which reads the current channel counts and prints them in the Serial Monitor
  * @param  stopOrRestart -> A boolean which specifies whether the communication window should remain open or be closed after transfer.
  *                           False keeps it open, true closes it. Use the STOP and RESTART definitions.
  * @retval None.
  * @notes  None.
  */
void IQS7222::printCounts(bool stopOrRestart)
{
    uint8_t transferBytes[10]; // Array to store the bytes transferred.
    readRandomBytes(CH0_COUNTS, 10, transferBytes, STOP);
   /* for (size_t i = 0; i < 9; i++)
    {
        Serial.print(transferBytes[i]);
        Serial.print(",");
    
    Serial.println(transferBytes[9]);*/
    Serial.print("CH1:");
    Serial.print(transferBytes[1]);
    Serial.print(",");
    Serial.print("CH2:");
    Serial.print(transferBytes[2]);
    Serial.print(",");
    Serial.print("CH3:");
    Serial.print(transferBytes[3]);
    Serial.print(",");
    Serial.print("CH6:");
    Serial.print(transferBytes[6]);
    Serial.print(",");
    Serial.print("CH7:");
    Serial.print(transferBytes[7]);
    Serial.print(",");
    Serial.print("CH8:");
    Serial.println(transferBytes[8]);
}

void IQS7222::readTest(const uint8_t length, uint16_t startRegister, bool stopOrRestart)
{
    uint8_t transferBytes[20]; // Array to store the bytes transferred.
    readRandomBytes(startRegister, length*2, transferBytes, stopOrRestart);
    for (size_t i = 0; i < length*2; i += 2)
    { 
        Serial.println((transferBytes[i + 1] << 8) + transferBytes[i]);
    }
}

void IQS7222::readSingleTest(uint16_t address, bool stopOrRestart)
{
    uint8_t transferBytes[2];
    readRandomBytes(address, 2, transferBytes, stopOrRestart);
    Serial.print("Data @0x");
    Serial.print(address, HEX);
    Serial.println(":");
    Serial.print("0x");
    Serial.println(transferBytes[0], HEX);
    Serial.print("0x");
    Serial.println(transferBytes[1], HEX);
}

/**
  * @name   getTouchEvents
  * @brief  A method which reads the events flags 
  * @param  stopOrRestart -> A boolean which specifies whether the communication window should remain open or be closed after transfer.
  *                           False keeps it open, true closes it. Use the STOP and RESTART definitions.
  * @retval None.
  * @notes  None.
  */
void IQS7222::getTouchEvents(bool stopOrRestart)
{
    uint8_t transferBytes[2];
    readRandomBytes(TOUCH_FLAGS, 2, transferBytes, stopOrRestart);

    uint16_t byteData = (transferBytes[1] << 8) + transferBytes[0];

    touch.flagByte = byteData;
}

/**
  * @name   setEventMask
  * @brief  A method which reads the events flags
  * @param  mask ->  Use the EVENT_MASK enumerator to specify the events to be enabled.
  *         numEvents -> An integer to indicate the number of elements in the mask array to be iterated upon to add to the EVENT_SETUP mask. 
  *         stopOrRestart -> A boolean which specifies whether the communication window should remain open or be closed after transfer.
  *                           False keeps it open, true closes it. Use the STOP and RESTART definitions.
  * @retval None.
  * @notes  None.
  */
void IQS7222::setEventMask(EVENT_MASK mask[], uint8_t numEvents, bool stopOrRestart)
{
    uint8_t transferBytes[2];
    
    readRandomBytes(EVENT_SETUP, 2, transferBytes, RESTART);
    
    transferBytes[0] &= ~0x3;
    transferBytes[1] &= ~0x30;

    for (int i = 0; i < numEvents; i++)
    {

        transferBytes[0] |= (mask[i] & 0xFF00);
        transferBytes[1] |= ((mask[i] & 0xFF) >> 8);

    }

    writeRandomBytes(EVENT_SETUP, 2, transferBytes, stopOrRestart);
    
    Serial.println((transferBytes[1] << 8) + transferBytes[0], BIN);
}

/**
  * @name   setInterface
  * @brief  A method which writes to the events flags
  * @param  mode ->  Use the INTERFACE_MODE enumerator to specify the mode to be enabled. 
  *         stopOrRestart -> A boolean which specifies whether the communication window should remain open or be closed after transfer.
  *                           False keeps it open, true closes it. Use the STOP and RESTART definitions.
  * @retval None.
  * @notes  Change pin to OUTPUT, pull LOW, delay, pull HIGH, change pin back to INPUT.
  */
void IQS7222::setInterface(INTERFACE_MODE mode, bool stopOrRestart)
{
    uint8_t transferBytes[1];
    readRandomBytes(CONTROL_SETTING, 1, transferBytes, RESTART);

    transferBytes[0] &= ~0xC0;
    transferBytes[0] |= mode;

    writeRandomBytes(CONTROL_SETTING, 1, transferBytes, stopOrRestart);
}

/**
  * @name   getEventFlags
  * @brief  A method which reads the event flags
  * @param  stopOrRestart -> A boolean which specifies whether the communication window should remain open or be closed after transfer.
  *                           False keeps it open, true closes it. Use the STOP and RESTART definitions.
  * @retval 16bit integer containing the different events - see EVENT_MASK for exact bit
  * @notes  None.
  */
uint16_t IQS7222::getEventFlags(bool stopOrRestart)
{
    uint8_t transferBytes[2];

    readRandomBytes(EVENT_FLAGS, 2, transferBytes, stopOrRestart);

    return uint16_t((transferBytes[1] << 8) + transferBytes[0]);
}

/**
  * @name   getTouchChannel
  * @brief  A method which reads the touch channel flags
  * @param  stopOrRestart -> A boolean which specifies whether the communication window should remain open or be closed after transfer.
  *                           False keeps it open, true closes it. Use the STOP and RESTART definitions.
  * @retval 16bit integer containing the flags for channel 0 through 9 - bit 10 to 15 are unassigned
  * @notes  None.
  */
uint16_t IQS7222::getTouchChannel(bool stopOrRestart)
{
    uint8_t transferBytes[2];

    readRandomBytes(TOUCH_FLAGS, 2, transferBytes, stopOrRestart);

    return uint16_t((transferBytes[1] << 8) + transferBytes[0]);
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

/**
 * @name    readRandomBytes
 * @brief   A methods which reads a specified number of bytes from a specified address and saves it into a user supplied array.
 *          This method is used by all other methods in this class which read data drom the IQS7222 device.
 * @param   memoryAddress -> The memory address at which to start reading bytes from. 
 *          numBytes      -> The number of bytes that must be read.
 *          bytesArray    -> The array which will store the bytes to be read, this array will be overwritten.
 *          stopOrRestart -> A boolean which specifies whether the communication window should remain open or be closed after transfer.
 *                           False keeps it open, true closes it. Use the STOP and RESTART definitions.
 * @retval  No value is returend, however, the user supplied array is overwritten.
 * @notes   Uses standard arduino "Wire" library which is for I2C communication.
 *          Take note that C++ cannot return an array, therefore, the array which is passed as an argument is overwritten with the required values.
 *          Pass an array to the method by using only its name, e.g. "bytesArray", without the brackets, this basically passes a pointer to the array.
 */
void IQS7222::readRandomBytes(uint16_t memoryAddress, uint8_t numBytes, uint8_t bytesArray[], bool stopOrRestart)
{
    uint8_t i = 0;  // A simple counter to assist with loading bytes into the user supplied array.

    // Select the device with the address of "_deviceAddress" and start communication.
    Wire.beginTransmission(_deviceAddress);
    
    // Verifies if 8bit or 16bit address
    if (memoryAddress <= 0xFF)
    {
        // Send a byte asking for the "memoryAddress" register 
        Wire.write(memoryAddress);
    }
    else
    {
        // Send two bytes asking for the "memoryAddress" register in little endian byte order
        Wire.write((memoryAddress & 0xFF00) >> 8);
        Wire.write(memoryAddress & 0xFF);

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

/**
  * @name   writeRandomBytes
  * @brief  A method which writes a specified number of bytes to a specified address, the bytes to write are supplied by means of an array pointer.
  *         This method is used by the all other methods of this class which write data to the IQS7222 device.
  * @param  memoryAddress -> The memory address at which to start writing the bytes to.
  *         numBytes      -> The number of bytes that must be written.
  *         bytesArray    -> The array which stores the bytes which will be written to the memory location.
  *         stopOrRestart -> A boolean which sepcifies whether the communication window should remain open or be closed of transfer.
  *                          False keeps it open, true closes it. Use the STOP and RESTART definitions.
  * @retval No value is returend, only the IQS device registers are altered.
  * @notes  Uses standard arduino "Wire" library which is for I2C communication.
  *         Take note that a full array cannot be passed to a function in C++.
  *         Pass an array to the function by using only its name, e.g. "bytesArray", without the square brackets, this basically passes a pointer to the array.
  *         The values to be written must be loaded into the array prior to passing it to the function.
  */
void IQS7222::writeRandomBytes(uint16_t memoryAddress, uint8_t numBytes, uint8_t bytesArray[], bool stopOrRestart)
{
    // Select the device with the address of "_deviceAddress" and start communication.
    Wire.beginTransmission(_deviceAddress);
    // Verifies if 8bit or 16bit address
    if (memoryAddress <= 0xFF)
    {
        // Send a byte asking for the "memoryAddress" register 
        Wire.write(memoryAddress);
    }
    else
    {
        // Send two bytes asking for the "memoryAddress" register in little endian byte order.
        Wire.write((memoryAddress & 0xFF00) >> 8);
        Wire.write(memoryAddress & 0xFF);
    }

    // Write the bytes as specified in the array which "arrayAddress" pointer points to.
    for (int i = 0; i < numBytes; i++)
    {
        Wire.write(bytesArray[i]);
    }
    // End the transmission, user decides to STOP or RESTART.
    Wire.endTransmission(stopOrRestart);
}


/**
 * @name    initialSetup
 * @brief   A methods which writes the exported parameter header file from the Azoteq proprietary tuning software. 
 * @param   stopOrRestart -> A boolean which specifies whether the communication window should remain open or be closed after transfer.
 *                           False keeps it open, true closes it. Use the STOP and RESTART definitions.
 * @retval  None.
 * @notes   None.
 */
void IQS7222::initialSetup(bool stopOrRestart)
{
    Serial.print("Writing initial Setup");
    uint8_t transferByte0[6] = { CYCLE_0_CONV_FREQ_FRAC, CYCLE_0_CONV_FREQ_PERIOD, CYCLE_0_SETTINGS, CYCLE_0_CTX_SELECT, CYCLE_0_IREF_0, CYCLE_0_IREF_1 };
    writeRandomBytes(CYCLE0_SETUP, 6, transferByte0, RESTART);
    uint8_t transferByte1[6] = { CYCLE_1_CONV_FREQ_FRAC, CYCLE_1_CONV_FREQ_PERIOD, CYCLE_1_SETTINGS, CYCLE_1_CTX_SELECT, CYCLE_1_IREF_0, CYCLE_1_IREF_1 };
    writeRandomBytes(CYCLE1_SETUP, 6, transferByte1, RESTART);
    uint8_t transferByte2[6] = { CYCLE_2_CONV_FREQ_FRAC, CYCLE_2_CONV_FREQ_PERIOD, CYCLE_2_SETTINGS, CYCLE_2_CTX_SELECT, CYCLE_2_IREF_0, CYCLE_2_IREF_1 };
    writeRandomBytes(CYCLE2_SETUP, 6, transferByte2, RESTART);
    uint8_t transferByte3[6] = { CYCLE_3_CONV_FREQ_FRAC, CYCLE_3_CONV_FREQ_PERIOD, CYCLE_3_SETTINGS, CYCLE_3_CTX_SELECT, CYCLE_3_IREF_0, CYCLE_3_IREF_1 };
    writeRandomBytes(CYCLE3_SETUP, 6, transferByte3, RESTART);
    uint8_t transferByte4[6] = { CYCLE_4_CONV_FREQ_FRAC, CYCLE_4_CONV_FREQ_PERIOD, CYCLE_4_SETTINGS, CYCLE_4_CTX_SELECT, CYCLE_4_IREF_0, CYCLE_4_IREF_1 };
    writeRandomBytes(CYCLE4_SETUP, 6, transferByte4, RESTART);

    uint8_t transferByte5[6] = { GLOBAL_CYCLE_SETUP_0,GLOBAL_CYCLE_SETUP_1, COARSE_DIVIDER_PRELOAD, FINE_DIVIDER_PRELOAD, COMPENSATION_PRELOAD_0, COMPENSATION_PRELOAD_1 };
    writeRandomBytes(GLOBAL_CYCLE_SETUP, 6, transferByte5, RESTART);
    
    uint8_t transferByte26[6] = { BUTTON_0_PROX_THRESHOLD, BUTTON_0_ENTER_EXIT, BUTTON_0_TOUCH_THRESHOLD, BUTTON_0_TOUCH_HYSTERESIS, BUTTON_0_PROX_EVENT_TIMEOUT, BUTTON_0_TOUCH_EVENT_TIMEOUT };
    writeRandomBytes(BUTTON0_SETUP, 6, transferByte26, RESTART);
    uint8_t transferByte27[6] = { BUTTON_1_PROX_THRESHOLD, BUTTON_1_ENTER_EXIT, BUTTON_5_TOUCH_THRESHOLD, BUTTON_5_TOUCH_HYSTERESIS, BUTTON_5_PROX_EVENT_TIMEOUT, BUTTON_5_TOUCH_EVENT_TIMEOUT };
    writeRandomBytes(BUTTON1_SETUP, 6, transferByte27, RESTART);
    uint8_t transferByte28[6] = { BUTTON_2_PROX_THRESHOLD, BUTTON_2_ENTER_EXIT, BUTTON_5_TOUCH_THRESHOLD, BUTTON_5_TOUCH_HYSTERESIS, BUTTON_5_PROX_EVENT_TIMEOUT, BUTTON_5_TOUCH_EVENT_TIMEOUT };
    writeRandomBytes(BUTTON2_SETUP, 6, transferByte28, RESTART);
    uint8_t transferByte29[6] = { BUTTON_3_PROX_THRESHOLD, BUTTON_3_ENTER_EXIT, BUTTON_5_TOUCH_THRESHOLD, BUTTON_5_TOUCH_HYSTERESIS, BUTTON_5_PROX_EVENT_TIMEOUT, BUTTON_5_TOUCH_EVENT_TIMEOUT };
    writeRandomBytes(BUTTON3_SETUP, 6, transferByte29,RESTART);
    uint8_t transferByte30[6] = { BUTTON_4_PROX_THRESHOLD, BUTTON_4_ENTER_EXIT, BUTTON_5_TOUCH_THRESHOLD, BUTTON_5_TOUCH_HYSTERESIS, BUTTON_5_PROX_EVENT_TIMEOUT, BUTTON_5_TOUCH_EVENT_TIMEOUT };
    writeRandomBytes(BUTTON4_SETUP, 6, transferByte30, RESTART);
    uint8_t transferByte6[6] = { BUTTON_5_PROX_THRESHOLD, BUTTON_5_ENTER_EXIT, BUTTON_5_TOUCH_THRESHOLD, BUTTON_5_TOUCH_HYSTERESIS, BUTTON_5_PROX_EVENT_TIMEOUT, BUTTON_5_TOUCH_EVENT_TIMEOUT };
    writeRandomBytes(BUTTON5_SETUP, 6, transferByte6, RESTART);
    uint8_t transferByte7[6] = { BUTTON_6_PROX_THRESHOLD, BUTTON_6_ENTER_EXIT, BUTTON_6_TOUCH_THRESHOLD, BUTTON_6_TOUCH_HYSTERESIS, BUTTON_6_PROX_EVENT_TIMEOUT, BUTTON_6_TOUCH_EVENT_TIMEOUT};
    writeRandomBytes(BUTTON6_SETUP, 6, transferByte7, RESTART);
    uint8_t transferByte8[6] = { BUTTON_7_PROX_THRESHOLD, BUTTON_7_ENTER_EXIT, BUTTON_7_TOUCH_THRESHOLD, BUTTON_7_TOUCH_HYSTERESIS, BUTTON_7_PROX_EVENT_TIMEOUT, BUTTON_7_TOUCH_EVENT_TIMEOUT };
    writeRandomBytes(BUTTON7_SETUP, 6, transferByte8, RESTART);
    uint8_t transferByte9[6] = { BUTTON_8_PROX_THRESHOLD, BUTTON_8_ENTER_EXIT, BUTTON_8_TOUCH_THRESHOLD, BUTTON_8_TOUCH_HYSTERESIS, BUTTON_8_PROX_EVENT_TIMEOUT, BUTTON_8_TOUCH_EVENT_TIMEOUT };
    writeRandomBytes(BUTTON8_SETUP, 6, transferByte9, RESTART);
    uint8_t transferByte10[6] = { BUTTON_9_PROX_THRESHOLD, BUTTON_9_ENTER_EXIT, BUTTON_9_TOUCH_THRESHOLD, BUTTON_9_TOUCH_HYSTERESIS, BUTTON_9_PROX_EVENT_TIMEOUT, BUTTON_9_TOUCH_EVENT_TIMEOUT };
    writeRandomBytes(BUTTON9_SETUP, 6, transferByte10, RESTART);

    uint8_t transferByte11[12] = { CH0_SETUP_0, CH0_SETUP_1, CH0_ATI_SETTINGS_0, CH0_ATI_SETTINGS_1, CH0_MULTIPLIERS_0, CH0_MULTIPLIERS_1, CH0_ATI_COMPENSATION_0, CH0_ATI_COMPENSATION_1, CH0_REF_PTR_0, CH0_REF_PTR_1, CH0_REFMASK_0, CH0_REFMASK_1};
    writeRandomBytes(CH0_GENERAL, 12, transferByte11, RESTART);
    uint8_t transferByte12[12] = { CH1_SETUP_0, CH1_SETUP_1, CH1_ATI_SETTINGS_0, CH1_ATI_SETTINGS_1, CH1_MULTIPLIERS_0, CH1_MULTIPLIERS_1, CH1_ATI_COMPENSATION_0, CH1_ATI_COMPENSATION_1, CH1_REF_PTR_0, CH1_REF_PTR_1, CH1_REFMASK_0, CH1_REFMASK_1 };
    writeRandomBytes(CH1_GENERAL, 12, transferByte12, RESTART);
    uint8_t transferByte13[12] = { CH2_SETUP_0, CH2_SETUP_1, CH2_ATI_SETTINGS_0, CH2_ATI_SETTINGS_1, CH2_MULTIPLIERS_0, CH2_MULTIPLIERS_1, CH2_ATI_COMPENSATION_0, CH2_ATI_COMPENSATION_1, CH2_REF_PTR_0, CH2_REF_PTR_1, CH2_REFMASK_0, CH2_REFMASK_1 };
    writeRandomBytes(CH2_GENERAL, 12, transferByte13, RESTART);
    uint8_t transferByte14[12] = { CH3_SETUP_0, CH3_SETUP_1, CH3_ATI_SETTINGS_0, CH3_ATI_SETTINGS_1, CH3_MULTIPLIERS_0, CH3_MULTIPLIERS_1, CH3_ATI_COMPENSATION_0, CH3_ATI_COMPENSATION_1, CH3_REF_PTR_0, CH3_REF_PTR_1, CH3_REFMASK_0, CH3_REFMASK_1 };
    writeRandomBytes(CH3_GENERAL, 12, transferByte14, RESTART);
    uint8_t transferByte15[12] = { CH4_SETUP_0, CH4_SETUP_1, CH4_ATI_SETTINGS_0, CH4_ATI_SETTINGS_1, CH4_MULTIPLIERS_0, CH4_MULTIPLIERS_1, CH4_ATI_COMPENSATION_0, CH4_ATI_COMPENSATION_1, CH4_REF_PTR_0, CH4_REF_PTR_1, CH4_REFMASK_0, CH4_REFMASK_1 };
    writeRandomBytes(CH4_GENERAL, 12, transferByte15, RESTART);
    uint8_t transferByte16[12] = { CH5_SETUP_0, CH5_SETUP_1, CH5_ATI_SETTINGS_0, CH5_ATI_SETTINGS_1, CH5_MULTIPLIERS_0, CH5_MULTIPLIERS_1, CH5_ATI_COMPENSATION_0, CH5_ATI_COMPENSATION_1, CH5_REF_PTR_0, CH5_REF_PTR_1, CH5_REFMASK_0, CH5_REFMASK_1 };
    writeRandomBytes(CH5_GENERAL, 12, transferByte16, RESTART);
    uint8_t transferByte17[12] = { CH6_SETUP_0, CH6_SETUP_1, CH6_ATI_SETTINGS_0, CH6_ATI_SETTINGS_1, CH6_MULTIPLIERS_0, CH6_MULTIPLIERS_1, CH6_ATI_COMPENSATION_0, CH6_ATI_COMPENSATION_1, CH6_REF_PTR_0, CH6_REF_PTR_1, CH6_REFMASK_0, CH6_REFMASK_1 };
    writeRandomBytes(CH6_GENERAL, 12, transferByte17, RESTART);
    uint8_t transferByte18[12] = { CH7_SETUP_0, CH7_SETUP_1, CH7_ATI_SETTINGS_0, CH7_ATI_SETTINGS_1, CH7_MULTIPLIERS_0, CH7_MULTIPLIERS_1, CH7_ATI_COMPENSATION_0, CH7_ATI_COMPENSATION_1, CH7_REF_PTR_0, CH7_REF_PTR_1, CH7_REFMASK_0, CH7_REFMASK_1 };
    writeRandomBytes(CH7_GENERAL, 12, transferByte18, RESTART);
    uint8_t transferByte19[12] = { CH8_SETUP_0, CH8_SETUP_1, CH8_ATI_SETTINGS_0, CH8_ATI_SETTINGS_1, CH8_MULTIPLIERS_0, CH8_MULTIPLIERS_1, CH8_ATI_COMPENSATION_0, CH8_ATI_COMPENSATION_1, CH8_REF_PTR_0, CH8_REF_PTR_1, CH8_REFMASK_0, CH8_REFMASK_1 };
    writeRandomBytes(CH8_GENERAL, 12, transferByte19, RESTART);
    uint8_t transferByte20[12] = { CH9_SETUP_0, CH9_SETUP_1, CH9_ATI_SETTINGS_0, CH9_ATI_SETTINGS_1, CH9_MULTIPLIERS_0, CH9_MULTIPLIERS_1, CH9_ATI_COMPENSATION_0, CH9_ATI_COMPENSATION_1, CH9_REF_PTR_0, CH9_REF_PTR_1, CH9_REFMASK_0, CH9_REFMASK_1 };
    writeRandomBytes(CH9_GENERAL, 12, transferByte20, RESTART);
    
    uint8_t transferByte21[4] = { COUNTS_BETA_FILTER, LTA_BETA_FILTER, LTA_FAST_BETA_FILTER, RESERVED_FILTER_0 };
    writeRandomBytes(FILTER_BETA, 4, transferByte21, RESTART);
    
    uint8_t transferByte22[20] = { SLIDER0SETUP_GENERAL, SLIDER0_LOWER_CAL, SLIDER0_UPPER_CAL, SLIDER0_BOTTOM_SPEED, SLIDER0_TOPSPEED_0, SLIDER0_TOPSPEED_1, SLIDER0_RESOLUTION_0, SLIDER0_RESOLUTION_1, SLIDER0_ENABLE_MASK_0_7, SLIDER0_ENABLE_MASK_8_9, SLIDER0_ENABLESTATUSLINK_0, SLIDER0_ENABLESTATUSLINK_1, SLIDER0_DELTA0_0, SLIDER0_DELTA0_1, SLIDER0_DELTA1_0, SLIDER0_DELTA1_1, SLIDER0_DELTA2_0, SLIDER0_DELTA2_1, SLIDER0_DELTA3_0, SLIDER0_DELTA3_1};
    writeRandomBytes(SLIDER0_GENERAL, 20, transferByte22, RESTART);
    uint8_t transferByte23[20] = { SLIDER1SETUP_GENERAL, SLIDER1_LOWER_CAL, SLIDER1_UPPER_CAL, SLIDER1_BOTTOM_SPEED, SLIDER1_TOPSPEED_0, SLIDER1_TOPSPEED_1, SLIDER1_RESOLUTION_0, SLIDER1_RESOLUTION_1, SLIDER1_ENABLE_MASK_0_7, SLIDER1_ENABLE_MASK_8_9, SLIDER1_ENABLESTATUSLINK_0, SLIDER1_ENABLESTATUSLINK_1, SLIDER1_DELTA0_0, SLIDER1_DELTA0_1, SLIDER1_DELTA1_0, SLIDER1_DELTA1_1, SLIDER1_DELTA2_0, SLIDER1_DELTA2_1, SLIDER1_DELTA3_0, SLIDER1_DELTA3_1 };
    writeRandomBytes(SLIDER1_GENERAL, 20, transferByte23, RESTART);

    uint8_t transferByte24[6] = { GPIO0_SETUP_0, GPIO0_SETUP_1, ENABLE_MASK_0_7, ENABLE_MASK_8_9, ENABLESTATUSLINK_0, ENABLESTATUSLINK_1 };
    writeRandomBytes(GPIO0_GENERAL, 6, transferByte24, RESTART);
    uint8_t transferByte25[21] = { SYSTEM_CONTROL_0, SYSTEM_CONTROL_1, ATI_ERROR_TIMEOUT_0, ATI_ERROR_TIMEOUT_1, ATI_REPORT_RATE_0, ATI_REPORT_RATE_1, NORMAL_MODE_TIMEOUT_0, NORMAL_MODE_TIMEOUT_1, NORMAL_MODE_REPORT_RATE_0, NORMAL_MODE_REPORT_RATE_1, LP_MODE_TIMEOUT_0, LP_MODE_TIMEOUT_1, LP_MODE_REPORT_RATE_0, LP_MODE_REPORT_RATE_1, ULP_MODE_TIMEOUT_0, ULP_MODE_TIMEOUT_1, ULP_MODE_REPORT_RATE_0, ULP_MODE_REPORT_RATE_1, TOUCH_PROX_EVENT_MASK, POWER_ATI_EVENT_MASK, I2CCOMMS_0 };
    writeRandomBytes(CONTROL_SETTING, 21, transferByte25, RESTART);

}   

