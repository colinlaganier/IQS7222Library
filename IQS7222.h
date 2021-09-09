/**
  **********************************************************************************
  * @file     IQS7222.h
  * @author   Colin Laganier
  * @version  V0.1
  * @date     2021-08-10
  * @brief   This file contains the header information for an IQS7222 Arduino library.
  *          The goal of the library is to provide the functionalities for touch sensing
  *			 to prototypes. Library derived from Azoteq's IQS266 library example.
  **********************************************************************************
  * @attention  Requires standard Arduino Libraries: Arduino.h, Wire.h.
  */


#ifndef IQS7222_H
#define IQS7222_H

// Include Files
#include "Arduino.h"
#include <Wire.h>
#include "IQS7222_addresses.h"

// Include initilisation files depending on prototype
#if defined(IQS7222_GALAXY)
#include "init/IQS7222C_galaxy_init.h"
#elif defined(IQS7222_MAGNETIC)
#include "init/IQS7222C_magnetic_init.h"
#endif

// Public Global Definitions
#define STOP true
#define RESTART false
#define BASE true
#define TARGET false

// Utility Bits
#define ACK_RESET_BIT 0x01
#define SHOW_RESET_BIT 0x08
#define DO_RESET_BIT 0x01
#define REDO_ATI_BIT 0x04

// Parameters
#define ACTIVITY_THRESHOLD 100

typedef union {
	uint16_t  flagByte;
	struct {
		uint8_t ch0 : 1;
		uint8_t ch1 : 1;
		uint8_t ch2 : 1;
		uint8_t ch3 : 1;
		uint8_t ch4 : 1;
		uint8_t ch5 : 1;
		uint8_t ch6 : 1;
		uint8_t ch7 : 1;
		uint8_t ch8 : 1;
		uint8_t ch9 : 1;
		uint8_t reserved : 6;
	} ;
	//uint8_t channel_array[10]{ch0, ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8, ch9};
} Touch_events;


typedef enum
{
	EMPTY = 0,
	CH1 = 1,
	CH2 = 6,
	CH3 = 2,
	CH4 = 7,
	CH5 = 3,
	CH6 = 8
} CHANNELS;

// Swipe directions
typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
} DIRECTION;

// Mask for the different events
typedef enum {
	POWER = 0x2000,
	ATI = 0x1000,
	TOUCH = 0x2,
	PROX = 0x1
} EVENT_MASK;

typedef enum {
	STREAM = 0x00,
	EVENT = 0x40,
	STREAM_TOUCH = 0x80
} INTERFACE_MODE;

class IQS7222
{
public:
	// Public constructor
	IQS7222();
	
	// Public Variables
	Touch_events touch;
	bool event_channel[10] = { false };
	//Gestures trackpadGestures; 
	size_t previousTouchRear = 0;
	size_t previousTouchIndex = 0;
	CHANNELS channelTest[10] = { EMPTY, CH1, CH3, CH5, EMPTY, EMPTY, CH2, CH4, CH6, EMPTY};
	CHANNELS previousTouch[10] = { CHANNELS::EMPTY };

	// Public methods
	bool begin(uint8_t deviceAddressIn, uint8_t readyPinIn);
	bool beginHeadless(uint8_t deviceAddressIn);
	bool requestComms(void);
	bool checkReset(bool stopOrRestart);
	void acknowledgeReset(bool stopOrRestart);
	void autoTune(bool stopOrRestart);
	void softReset(bool stopOrRestart);
	void printCounts(bool stopOrRestart);
	void getTouchEvents(bool stopOrRestart);
	void setEventMask(EVENT_MASK mask[], uint8_t numEvents, bool stopOrRestart);
	void setInterface(INTERFACE_MODE mode, bool stopOrRestart);
	uint16_t getEventFlags(bool stopOrRestart);
	uint16_t getTouchChannel(bool stopOrRestart);
	void ackowledgeEvent(bool stopOrRestart);
	void verifyEvent(bool stopOrRestart);
	void setAtiValues(bool baseOrTarget, uint8_t channel, uint8_t value, bool stopOrRestart);
	void setAtiValues(bool baseOrTarget, uint8_t channel[], uint8_t numChannels, uint8_t value, bool stopOrRestart);
	void addTouch(void);
	void clearTouch(void);
	void gestureUpdate(void);
	DIRECTION identifySwipe(void);


private:
	// Private variables
	uint8_t _deviceAddress;
	uint8_t _readyPin;

	// Private methods
	void toggleReady(void);
	void readRandomBytes(uint16_t memoryAddress, uint8_t numBytes, uint8_t bytesArray[], bool stopOrRestart);
	void writeRandomBytes(uint16_t memoryAddress, uint8_t numBytes, uint8_t bytesArray[], bool stopOrRestart);
	void initialSetup(bool stopOrRestart);
	bool compareCounts(uint8_t counts[], uint8_t LTA[], uint8_t numChannels, uint8_t startChannel);
	bool verifyPreviousTouch();
};

#endif