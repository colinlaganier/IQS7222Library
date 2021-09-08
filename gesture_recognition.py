#!/usr/bin/env python3

import serial 
import sys
import time
import os

# Serial communication variables 
baud_rate = 9600
COM_port = "COM11"

# Gesture recognition variables
monitor = False
init = False
previousTouch = [0]*10
previousTouchIndex = 0
previousTouchRear = 0
currentTime = 0;
previousTime = 0;
noGestureCounter = 0
gestureResult = ["None", "Up", "Down", "Left", "Right"]

# Gesture recognition methods 
def clearPrevious():
	previousTouch = [0]*10
	previousTouchIndex = previousTouchRear = 0;
	currentTime = None;

# Get current time
def current_milli_time():
    return round(time.time() * 1000)

# Get index of next touch
def nextIndex():
	global previousTouchRear
	if (previousTouchIndex < 9):
		return previousTouchIndex + 1
	else: 
		previousTouchRear += 1
		return (previousTouchRear - 1)

# Return the next index
def loopIndex(index):
	if (index > 0):
		return (index - 1)
	else:
		return 9

# Loops through the previous touch events to identify a vertical gesture
def verticalGesture(midChannel, outChannel, output):
	_firstIndex = (previousTouchIndex - 1)	# 
	while (_firstIndex != previousTouchRear):
		if (previousTouch[_firstIndex] == midChannel):
			_secondIndex = loopIndex(_firstIndex)
			while (_secondIndex != previousTouchRear):
				if (previousTouch[_secondIndex] == outChannel):
					return output
				else:
					_secondIndex = loopIndex(_secondIndex)
		else:
			_firstIndex = loopIndex(_firstIndex)
	return 0

# Loops through the previous touch events to identify a horizontal gesture
def horizontalGesture(outChannel, output):
	_firstIndex = (previousTouchIndex - 1)
	while (_firstIndex != previousTouchRear):
		if (previousTouch[_firstIndex] == outChannel):
			return output
		else:
			_firstIndex = loopIndex(_firstIndex);
	return 0

# Identify swipe gesture direction from the previous touch events
def identifyGesture():

	direction = 0

	# Swipe UP left receiving electrode
	if ((previousTouch[previousTouchIndex] - 1) == 5 ):	# Verifies if last read event matches gesture
		direction = verticalGesture(3,1,1)

	# Swipe UP right receiving electrode
	elif ((previousTouch[previousTouchIndex] - 1) == 6 ):	# Verifies if last read event matches gesture
		direction = verticalGesture(4,2,1)

		# Swipe DOWN left receiving electrode
	elif ((previousTouch[previousTouchIndex] - 1) == 1 ):	# Verifies if last read event matches gesture
		direction = verticalGesture(3,5,2)

		# Swipe DOWN right receiving electrode
	elif ((previousTouch[previousTouchIndex] - 1) == 2 ):	# Verifies if last read event matches gesture
		direction = verticalGesture(4, 6, 2)

	if (direction != 0):
		return direction

	_rightCounter = 0
	_leftCounter = 0

	# Swipe RIGHT bottom
	if ((previousTouch[previousTouchIndex] - 1) == 2 ):
		direction[outputCounter] = horizontalGesture(1, 4)
		if (direction[outputCounter] != 0):
			_rightCounter+= 1

	# Swipe RIGHT mid
	elif ((previousTouch[previousTouchIndex] - 1) == 3 ):
		direction[outputCounter] = horizontalGesture(4, 4)
		if (direction[outputCounter] != 0):
			_rightCounter += 1

	# Swipe RIGHT top
	elif ((previousTouch[previousTouchIndex] - 1) == 5 ):
		direction[outputCounter] = horizontalGesture(6, 4)
		if (direction[outputCounter] != 0):
			_rightCounter += 1

	# Swipe LEFT bottom
	elif ((previousTouch[previousTouchIndex] - 1) == 1 ):
		direction[outputCounter] = horizontalGesture(2, 3)
		if (direction[outputCounter] != 0):
			_leftCounter += 1

	# Swipe LEFT mid
	elif ((previousTouch[previousTouchIndex] - 1) == 4 ):
		direction[outputCounter] = horizontalGesture(3, 3)
		if (direction[outputCounter] != 0):
			_leftCounter += 1

	# Swipe LEFT top
	elif ((previousTouch[previousTouchIndex] - 1) == 6 ):
		direction[outputCounter] = horizontalGesture(5, 3)
		if (direction[outputCounter] != 0):
			_leftCounter += 1

	if ((_rightCounter + _leftCounter) != 0):
		if (_rightCounter > _leftCounter):
			return 4
		elif (_leftCounter > _rightCounter):
			return 3

	return 0



if sys.argv[1].isnumeric():
	COM_port = "COM" + sys.argv[1]
	print(COM_port)
	monitor = True
elif __name__ == "__main__":
	monitor = True

print("Initialising serial communuication with COM port" + COM_port)
ser = serial.Serial(COM_port, baud_rate, timeout = 0.1)

while True:

	val =  str(ser.readline().decode('utf-8', errors='replace').strip('\r\n'))# Serial read
	print(val)
	if (monitor == True):

		_channelId = int(val[val.find("TouchChannel:")+13])

		if (_channelId != 0):
			previousTime = currentTime; 
			currentTime = current_milli_time()
		
			if ((currentTime - previousTime ) < 500):	# Verifies no inactivity period 
				print(val[10])
				previousTouch[previousTouchIndex] = _channelId	# Get channel with touch event
				result = identifyGesture()
				previousTouchIndex = nextIndex()
				if (result != 0):
					print(f"SwipeGesture:{gestureResult[result]}")
					clearPrevious()
				else:	#To make sure the program is running
					if (noGestureCounter == 49):
						Serial.print('.\n')
					else:
						print('.')
		
			elif (previousTime != None):
				clearPrevious()


