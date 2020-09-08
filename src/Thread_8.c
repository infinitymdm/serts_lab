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

//===== GLOBAL DEFINITIONS =====//
#define LED_Green   0
#define LED_Orange  1
#define LED_Red     2
#define LED_Blue    3

//========= PROTOTYPES =========//
void delay(void);
void Thread_1 (void const *arg);
void Thread_2 (void const *arg);

//==== THREAD DEFINITIIONS =====//
osThreadDef (Thread_1, osPriorityNormal, 1, 0);
osThreadDef (Thread_2, osPriorityNormal, 1, 0);

osSemaphoreDef (SEM0);
osSemaphoreId (SEM0_id);

//==== FUNCTION DEFINITIONS ====//
void Init_Thread (void) {
	// Thread IDs
	osThreadId id_1;
	osThreadId id_2;

	LED_Initialize(); // Initialize the LEDs
	UART_Init(); // Initialize the UART

	SEM0_id = osSemaphoreCreate(osSemaphore (SEM0), 1);

	id_1 = osThreadCreate (osThread (Thread_1), NULL);	// create thread_1
	id_2 = osThreadCreate (osThread (Thread_2), NULL);	// create thread_2
	// handle thread creation
	if (id_1 == NULL || id_2 == NULL) {
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
	LED_On(0);
	delay();
	osSemaphoreRelease(SEM0_id); // release semaphore
	LED_Off(0);
	delay();
	osSemaphoreWait(SEM0_id, osWaitForever); // wait on the semaphore
	LED_On(0);
	delay();
	osSemaphoreWait(SEM0_id,osWaitForever);
	osSemaphoreWait(SEM0_id,osWaitForever);
	LED_Off(0);
	delay();
	osSemaphoreRelease(SEM0_id);
	LED_On(0);
	delay();

}

void Thread_2(void const *arg){
	LED_On(1);
	delay();
	osSemaphoreRelease(SEM0_id);
	LED_Off(1);
	delay();
	osSemaphoreWait(SEM0_id,osWaitForever);
	osSemaphoreRelease(SEM0_id);
	osSemaphoreWait(SEM0_id,osWaitForever);
	LED_On(1);
	delay();
}
