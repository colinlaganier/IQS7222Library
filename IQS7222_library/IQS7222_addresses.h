/**
  **********************************************************************************
  * @file     IQS7222_addresses.h
  * @author   Colin Laganier
  * @version  V0.1
  * @date     2021-08-10
  * @brief   This file contains the registers used in the IQS7222 library for touch.
  **********************************************************************************
  */
  
/**************************************************************************************************************/
/*                                        IQS7222 - REGISTER & MEMORY MAP                                     */
/**************************************************************************************************************/

// System flags
#define SYS_FLAGS 0x10	//Byte 0: FLAGS		Byte 1: Reserved
// Events flags
#define EVENT_FLAGS 0x11
#define PROX_FLAGS 0x12
#define TOUCH_FLAGS 0x13
#define SLIDER1_OUTPUT 0x14	//16-bit int
#define SLIDER2_OUTPUT 0x15	//16-bit int

// Channel Counts
#define CH0_COUNTS 0x20	//16-bit int
#define CH1_COUNTS 0x21
#define CH2_COUNTS 0x22
#define CH3_COUNTS 0x23
#define CH4_COUNTS 0x24
#define CH5_COUNTS 0x25
#define CH6_COUNTS 0x26
#define CH7_COUNTS 0x27
#define CH8_COUNTS 0x28
#define CH9_COUNTS 0x29

// Channel LTA
#define CH0_LTA 0x30	//16-bit int
#define CH1_LTA 0x31
#define CH2_LTA 0x32
#define CH3_LTA 0x33
#define CH4_LTA 0x34
#define CH5_LTA 0x35
#define CH6_LTA 0x36
#define CH7_LTA 0x37
#define CH8_LTA 0x38
#define CH9_LTA 0x39

// Cycle Setup
#define CYCLE0_SETUP 0x8000
#define CYCLE1_SETUP 0x8100
#define CYCLE2_SETUP 0x8200
#define CYCLE3_SETUP 0x8300
#define CYCLE4_SETUP 0x8400
// Cycle 0 Setup
#define CYCLE0_SETUP0 0x8000	//Byte 0: Conversion Period		Byte 1: Frequency Fraction
#define CYCLE0_SETUP1 0x8001
#define CYCLE0_SETUP2 0x8002
// Cycle 1 Setup
#define CYCLE1_SETUP0 0x8100
#define CYCLE1_SETUP1 0x8101
#define CYCLE1_SETUP2 0x8102
// Cycle 2 Setup
#define CYCLE2_SETUP0 0x8200
#define CYCLE2_SETUP1 0x8201
#define CYCLE2_SETUP2 0x8202
// Cycle 3 Setup
#define CYCLE3_SETUP0 0x8300
#define CYCLE3_SETUP1 0x8301
#define CYCLE3_SETUP2 0x8302
// Cycle 4 Setup
#define CYCLE4_SETUP0 0x8400
#define CYCLE4_SETUP1 0x8401
#define CYCLE4_SETUP2 0x8402

#define GLOBAL_CYCLE_SETUP 0x8500
#define MULTIPLIER_PRELOAD 0x8501
#define COMPENSATION_PRELOAD 0x502

// Button Setup
#define BUTTON0_SETUP 0x9000
#define BUTTON1_SETUP 0x9100
#define BUTTON2_SETUP 0x9200
#define BUTTON3_SETUP 0x9300
#define BUTTON4_SETUP 0x9400
#define BUTTON5_SETUP 0x9500
#define BUTTON6_SETUP 0x9600
#define BUTTON7_SETUP 0x9700
#define BUTTON8_SETUP 0x9800
#define BUTTON9_SETUP 0x9900

// Channel Setup

