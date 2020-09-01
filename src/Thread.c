
#include "cmsis_os.h"  // CMSIS RTOS header file
#include "Board_LED.h"
#include "UART_driver.h"
#include "stdint.h"                     // data type definitions
#include "stdio.h"                      // file I/O functions
#include "rl_usb.h"                     // Keil.MDK-Pro::USB:CORE
#include "rl_fs.h"                      // Keil.MDK-Pro::File System:CORE
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_audio.h"
#include "math.h"
#include "arm_math.h" // header for DSP library
#include <stdio.h>

// LED constants
#define LED_Green   0
#define LED_Orange  1
#define LED_Red     2
#define LED_Blue    3

// PROTOTYPES
void delay(void)
void Thread_1 (void const *arg);                           // function prototype for Thread_1
osThreadDef (Thread_1, osPriorityNormal, 1, 0);            // define Thread_1

// FUNCTION DEFINITIONS
void Init_Thread (void) {

	osThreadId id; // holds the returned thread create ID
	
	LED_Initialize(); // Initialize the LEDs
	UART_Init(); // Initialize the UART
	
  id = osThreadCreate (osThread (Thread_1), NULL);         // create the thread
  if (id == NULL) {                                        // handle thread creation
    // Failed to create a thread
  };
}

// Custom delay function
void delay(void){
	int32_t j;
	for (j=0; j<20000000; j++){};
}

// Thread function
void Thread_1(void const *arg){

	}
