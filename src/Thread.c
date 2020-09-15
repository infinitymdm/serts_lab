
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

// Definitions
#define LED_Green   0
#define LED_Orange  1
#define LED_Red     2
#define LED_Blue    3
#define RX_LEN 		5

// PROTOTYPES
void delay(void);
void Thread_1 (void const *argument);           // thread function 1
osThreadId tid_Thread_1;                      	// thread id 1
osThreadDef (Thread_1, osPriorityNormal, 1, 0); // thread object 1

// FUNCTION DEFINITIONS
void delay(void){
	int32_t j;
  	for(j=0;j<20000000;j++){};
}

int Init_Thread (void) {
    LED_Initialize(); // Initialize the LEDs
    UART_Init(); // Initialize the UART

    tid_Thread_1 = osThreadCreate (osThread(Thread_1), NULL);
    if (!tid_Thread_1) return(-1);

    return(0);
}

void Thread_1 (void const *argument) {
    while (1) {
		LED_On(0);
		delay();
		LED_Off(0);
		delay();
    }
}

