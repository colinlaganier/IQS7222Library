/**
  **********************************************************************************
  * @file     gestures.cpp
  * @author   Colin Laganier
  * @version  V0.1
  * @date     2021-08-27
  * @brief	  This file contains the constructors and methods for gesture recognition using
  *			  the IQS7222C for mutual projected capacitive sensing. Designed for six
  *			  channel sensing.
  **********************************************************************************
  */

#include "gestures.h"


/**************************************************************************************************************/
/*                                                CONSTRUCTORS                                                */
/**************************************************************************************************************/
Gestures::Gestures() {}



/**************************************************************************************************************/
/*                                              PUBLIC METHODS                                                */
/**************************************************************************************************************/

//void Gestures::enQueue(int8_t value)
//{
//	if (rear == SIZE - 1)
//	{
//		Serial.println("Queue is full");
//		//return false;
//	}
//	else
//	{
//		if (front == -1)
//		{
//			front = 0;
//		}
//		rear++;
//		items[rear] = value;
//		Serial.println((String)"inserted " + value);
//		//return true;
//	}
//}
//
//void Gestures::deQueue()
//{
//	if (rear == -1)
//	{
//		Serial.println("Queue is empty");
//		//return false; 
//	}
//	else 
//	{
//		Serial.println((String)"deleted " + value);
//		front++;
//		if (front > rear)
//		{
//			front--;
//			rear--;
//		}
//		//return true; 
//	}
//}
//
//void Gestures::printQueue()
//{
//	for (int8_t i = front; i < rear; i++)
//	{
//		Serial.print((String)items[i] + " ");
//	}
//	Serial.print("\n");
//}



void Gestures::updateCount(uint8_t transferBytes[])
{
	for (size_t i = 0; i < NUMBER_CHANNELS; i++)
		queue_channels[i][queue_index] = ((uint16_t)(transferBytes[2 * i + 1] << 8) + transferBytes[2 * i]);
	
	if (!queue_filled)
	{
		queue_index++;
		if (queue_index = QUEUE_SIZE)
		{
			queue_filled = true;
			queue_index = 0;
		}
	}
	else
	{
		if (queue_index == QUEUE_SIZE - 1)
			queue_index = 0;
		else
			queue_index++;
	}

}

void Gestures::updateLTA(uint8_t transferBytes[])
{
	for (size_t i = 0; i < NUMBER_CHANNELS; i++)
	{
		channel_LTA[i] = (transferBytes[2 * i + 1] << 8) + transferBytes[2 * i];
	}
}

void Gestures::fitSlope()
{
	if (queue_filled)
	{
		for (size_t i = 0; i < 6; i++)
		{
			for (auto& count : queue_channels[i])
			{
				count_average[i] += count;
			}
			count_average[i] /= QUEUE_SIZE;
			uint16_t count_sum = 0;
			
			for (size_t j = 0; j < QUEUE_SIZE; j++)
			{
				count_sum += (queue_channels[i][j]-count_average[i])*x_values[j];
			}
			channel_slope[i] = count_sum / 2;
		}
	}

}

void Gestures::identifySwipe(CHANNELS ch1, CHANNELS ch2)
{
	if (channel_slope[ch1] > 0)
	{
		if (channel_slope[ch2] < 0)
		{
			queue_channels[ch1][queue_index]
		}
	}
}



void Gestures::identifyGesture()
{
	bool channel_activity[NUMBER_CHANNELS] = { false };
	for (size_t i = 0; i < NUMBER_CHANNELS; i++)
	{
		//if (queue_channels[i][(queue_index + 2) % 3] - (channel_LTA[i] + COUNT_THRESHOLD) > 0)
		if (queue_channels[i][(queue_index + 2) % 3] - (channel_LTA[i] + COUNT_THRESHOLD) > 0)
		{
			channel_activity[i] = true;
		}
	}
	if ()
}

CHANNELS Gestures::identifySwipe()
{
	if 
	return CHANNELS();
}



