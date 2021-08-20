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
//#include "Wire\src\Wire.h"
#include <Wire.h>
#include "IQS7222_addresses.h"

// Public Global Definitions
#define STOP true
#define RESTART false

// Utility Bits
#define ACK_RESET_BIT 0x01
#define SHOW_RESET_BIT 0x08
#define DO_RESET_BIT 0x01
#define REDO_ATI_BIT 0x04

typedef enum {
	CH_0,
	CH_1,
	CH_2,
	CH_3,
	CH_4,
	CH_5,
	CH_6
} CHANNEL;

struct CHANNELS {
	uint16_t CH_0 : CH0_GENERAL;
	uint16_t CH_1 : CH1_GENERAL;
	uint16_t CH_2 : CH2_GENERAL;
	uint16_t CH_3 : CH3_GENERAL;
	uint16_t CH_4 : CH4_GENERAL;
	uint16_t CH_5 : CH5_GENERAL;
	uint16_t CH_6 : CH6_GENERAL;
	uint16_t CH_7 : CH7_GENERAL;
	uint16_t CH_8 : CH8_GENERAL;
	uint16_t CH_9 : CH9_GENERAL;
};

struct channel_general
{
	uint16_t MODE : 0xC000;
	uint16_t ATI_BAND : 0x3000;
	uint16_t GLOBAL_HALT : 0x800;
	uint16_t INVERT : 0x400;
	uint16_t DUAL : 0x200;
	uint16_t ENABLED : 0x100;
	uint16_t CRX3 : 0x80;
	uint16_t CRX2 : 0x40;
	uint16_t CRX1 : 0x20;
	uint16_t CRX0 : 0x10;
	uint16_t CS_SIZE : 0x08;
	uint16_t VBIAS : 0x04;
	uint16_t PROJECTED_BIAS : 0x03;
};

struct channel_ATI
{
	uint16_t ATI_TARGET : 0xFF00;
	uint16_t ATI_BASE : 0xF8;
	uint16_t ATI_MODE : 0x07;
};

struct channel_multipliers
{
	uint16_t FINE_DIVIDER : 0x3E00;
	uint16_t COARSE_MULTIPLIER : 0x1E0;
	uint16_t COARSE_DIVIDER : 0x1F;
};

struct channel_ATI_compensation
{
	uint16_t COMPENSATION_DIVIDER : 0xF800;
	uint16_t COMPENSATION_SELECTION : 0x3FF;
};

class IQS7222
{
public:
	// Public constructor
	IQS7222();

	//Public variables

	// Public methods
	bool begin(uint8_t deviceAddressIn, uint8_t readyPinIn);
	bool beginHeadless(uint8_t deviceAddressIn);
	bool requestComms(void);
	bool checkReset(bool stopOrRestart);
	void acknowledgeReset(bool stopOrRestart);
	void autoTune(bool stopOrRestart);
	void softReset(bool stopOrRestart);
	void printCounts(bool stopOrRestart);
	//void channelSettings(CHANNEL channel, );

private:
	// Private variables
	uint8_t _deviceAddress;
	uint8_t _readyPin;

	// Private methods
	void toggleReady(void);
	void readRandomBytes(uint16_t memoryAddress, uint8_t numBytes, uint8_t bytesArray[], bool stopOrRestart);
	void writeRandomBytes(uint16_t memoryAddress, uint8_t numBytes, uint8_t bytesArray[], bool stopOrRestart);
	void initialSetup(bool stopOrRestart);
};

#endif