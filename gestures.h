/**
  **********************************************************************************
  * @file     gestures.h
  * @author   Colin Laganier
  * @version  V0.1
  * @date     2021-08-27
  * @brief	  This file contains the header information for gesture recognition using
  *			  the IQS7222C for mutual projected capacitive sensing. Designed for six
  *			  channel sensing.
  **********************************************************************************
  */

#ifndef GESTURES_H
#define GESTURES_H

#include "IQS7222.h"

#define QUEUE_SIZE 3
#define NUMBER_CHANNELS 6
#define COUNT_THRESHOLD 100

typedef enum
{
	CH0,
	CH1,
	CH2,
	CH3,
	CH4,
	CH5, 
	CH6
} CHANNELS;

class Gestures: public IQS7222
{	
	// Public constructor
	Gestures();

public:
	
	// Public variables
	uint16_t queue_channels[NUMBER_CHANNELS][QUEUE_SIZE] = { 0 };
	uint8_t count_average[6] = { 0 };
	uint16_t channel_LTA[NUMBER_CHANNELS] = { 0 };
	uint8_t channel_slope[NUMBER_CHANNELS] = { 0 };
	uint8_t queue_index = 0;
	bool queue_filled = false; 
	uint8_t x_values[3] = { -1, 0, 1 };
	uint8_t recentTouch[10] = { 0 };

	// Public methods
	void updateCount(uint8_t transferBytes[]);
	void updateLTA(uint8_t transferBytes[]);
	void fitSlope();
	void identifySwipe(CHANNELS ch1, CHANNELS ch2);
	void identifyGesture();
	void addTouch();
	CHANNELS identifySwipe();

private:

};

#endif

