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
// Channel 0 Setup
#define CH0_GENERAL 0xA000
#define CH0_ATI 0xA001
#define CH0_MULTIPLIERS 0xA002
#define CH0_REF_SETTINGS0 0xA003
#define CH0_REF_SETTINGS1 0xA004
// Channel 1 Setup
#define CH1_GENERAL 0xA100
#define CH1_ATI 0xA101
#define CH1_MULTIPLIERS 0xA102
#define CH1_REF_SETTINGS0 0xA103
#define CH1_REF_SETTINGS1 0xA104
// Channel 2 Setup
#define CH2_GENERAL 0xA200
#define CH2_ATI 0xA201
#define CH2_MULTIPLIERS 0xA202
#define CH2_REF_SETTINGS0 0xA203
#define CH2_REF_SETTINGS1 0xA204
// Channel 3 Setup
#define CH3_GENERAL 0xA300
#define CH3_ATI 0xA301
#define CH3_MULTIPLIERS 0xA302
#define CH3_REF_SETTINGS0 0xA303
#define CH3_REF_SETTINGS1 0xA304
// Channel 4 Setup
#define CH4_GENERAL 0x400
#define CH4_ATI 0x401
#define CH4_MULTIPLIERS 0x402
#define CH4_REF_SETTINGS0 0x403
#define CH4_REF_SETTINGS1 0x404
// Channel 5 Setup
#define CH5_GENERAL 0xA500
#define CH5_ATI 0xA501
#define CH5_MULTIPLIERS 0xA502
#define CH5_REF_SETTINGS0 0xA503
#define CH5_REF_SETTINGS1 0xA504
// Channel 6 Setup
#define CH6_GENERAL 0xA600
#define CH6_ATI 0xA601
#define CH6_MULTIPLIERS 0xA602
#define CH6_REF_SETTINGS0 0xA603
#define CH6_REF_SETTINGS1 0xA604
// Channel 2 Setup
#define CH7_GENERAL 0xA700
#define CH7_ATI 0xA701
#define CH7_MULTIPLIERS 0xA702
#define CH7_REF_SETTINGS0 0xA703
#define CH7_REF_SETTINGS1 0xA704
// Channel 3 Setup
#define CH8_GENERAL 0xA800
#define CH8_ATI 0xA801
#define CH8_MULTIPLIERS 0xA802
#define CH8_REF_SETTINGS0 0xA803
#define CH8_REF_SETTINGS1 0xA804
// Channel 9 Setup
#define CH9_GENERAL 0x900
#define CH9_ATI 0x901
#define CH9_MULTIPLIERS 0x902
#define CH9_REF_SETTINGS0 0x903
#define CH9_REF_SETTINGS1 0x904

// Filter Betas 
#define FILTER_BETA 0xAA00
#define FAST_FILTER_BETA 0xAA01

// Slider Settings
// Slider 0 Setup
#define SLIDER0_GENERAL 0xB000
#define SLIDER0_CALIBRATION 0xB001
#define SLIDER0_SPEED 0xB002		//16-bit int
#define SLIDER0_RESOLUTION 0xB003	//16-bit int
#define SLIDER0_MASK 0xB004
#define SLIDER0_STATUS 0xB005
#define SLIDER0_DELTA0 0xB006
#define SLIDER0_DELTA1 0xB007
#define SLIDER0_DELTA2 0xB008
#define SLIDER0_DELTA3 0xB009
// Slider 1 Setup
#define SLIDER1_GENERAL 0xB100
#define SLIDER1_CALIBRATION 0xB101
#define SLIDER1_SPEED 0xB102		//16-bit int
#define SLIDER1_RESOLUTION 0xB103	//16-bit int
#define SLIDER1_MASK 0xB104
#define SLIDER1_STATUS 0xB105
#define SLIDER1_DELTA0 0xB106
#define SLIDER1_DELTA1 0xB107
#define SLIDER1_DELTA2 0xB108
#define SLIDER1_DELTA3 0xB109

// GPIO0 Settings
#define GPIO0_GENERAL 0xC000
#define GPIO0_MASK 0xC004
#define GPIO0_STATUS 0xC005

// PMU & System Settings
#define CONTROL_SETTING 0xD0
#define ATI_TIMEOUT 0xD1		//16-bit int
#define ATI_REPORT 0xD2			//16-bit int
#define NP_TIMEOUT 0xD3			//16-bit int
#define NP_REPORT 0xD4			//16-bit int
#define LP_TIMEOUT 0xD5			//16-bit int
#define LP_REPORT 0xD6			//16-bit int
#define	ULP_UPDATE_RATE 0xD7	//16-bit int
#define ULP_REPORT 0xD8			//16-bit int
#define EVENT_SETUP 0xD9
#define I2C_SETUP 0xDA