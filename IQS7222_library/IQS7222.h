/**
  **********************************************************************************
  * @file     IQS7222.h
  * @author   Colin Laganier
  * @version  V0.1
  * @date     2021-08-10
  * @brief   This file contains the header information for an IQS7222 Arduino library.
  *          The goal of the library is to provide the functionalities for touch sensing
  *			 to prototypes. Library derived from Azoteq's IQS266 example.
  **********************************************************************************
  * @attention  Requires standard Arduino Libraries: Arduino.h, Wire.h.
  */


#ifndef IQS7222_h
#define IQS7222_h

  // Include Files
#include "Arduino.h"
#include "Wire\src\Wire.h"
#include "IQS7222_addresses.h"

#define STOP true
#define RESTART false

class IQS7222
{
public:
	// Public methods
	bool begin(uint8_t deviceAddressIn, uint8_t readyPinIn);
	bool beginHeadless(uint8_t deviceAddressIn);
	bool requestComms(void);
	bool checkReset(bool stopOrRestart);
	void acknowledgeReset(bool stopOrRestart);
	void autoTune(bool stopOrRestart);

private:
	// Private variables
	uint8_t _deviceAddress;
	uint8_t _readyPin;

	// Private methods
	void toggleReady(void);
	void readRandomBytes(uint16_t memoryAddress, uint8_t numBytes, uint8_t bytesArray[], bool stopOrRestart);
	void writeRandomBytes(uint16_t memoryAddress, uint8_t numBytes, uint8_t bytesArray[], bool stopOrRestart);

};

#endif