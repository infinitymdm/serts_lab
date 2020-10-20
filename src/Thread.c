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
#include "arm_math.h"
#include <stdio.h>

#define LED_Green   0
#define LED_Orange  1
#define LED_Red     2
#define LED_Blue    3

#define NUM_CHAN	2 // number of audio channels
#define NUM_POINTS 1024 // number of points per channel
#define BUF_LEN NUM_CHAN*NUM_POINTS // length of the audio buffer

// Semaphores
osSemaphoreDef (SEM0);
osSemaphoreId (SEM0_id);

// Message Queues
osMessageQId mid_bufQueue;
osMessageQDef(bufQueue, 1, uint32_t);

// Threads
void Thread_1 (void const *argument); // Thread_1 prototype
osThreadId tid_Thread_1;
osThreadDef(Thread_1, osPriorityNormal, 1, 0);

// buffer used for audio play
int16_t Audio_Buffer_1[BUF_LEN];
int16_t Audio_Buffer_2[BUF_LEN];

void Init_Thread (void) {
	LED_Initialize(); // Initialize the LEDs
	UART_Init(); // Initialize the UART

	// Create semaphores
	SEM0_id = osSemaphoreCreate(osSemaphore(SEM0), 0);

	// Create queues
	mid_bufQueue = osMessageCreate(osMessageQ(bufQueue), NULL);

	// Create threads
	tid_Thread_1 = osThreadCreate(osThread(Thread_1), NULL);
	if (!tid_Thread_1) return;

	if (!mid_bufQueue || !tid_Thread_1) return;
}

void Thread_1 (void const *argument) {
	static uint8_t rtrn = 0;  	// return variable
	float32_t Fs = 8000.0; 		// sample frequency
	float32_t freq = 200.0; 	// signal frequency in Hz
	int16_t i; 					// loop counter
	float32_t tmp; 				// factor
	float32_t cnt = 0.0f; 		// time index

	tmp = 6.28f*freq/Fs; // only calc this factor once

	// initialize the audio output
	rtrn = BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 0x46, 8000);
	if (rtrn != AUDIO_OK) return;

	// generate data for the audio buffer
 	for (i=0;i<NUM_POINTS;i++) {
		Audio_Buffer_1[2*i] = arm_cos_f32(tmp*cnt)*32000.0f; // Left channel
		Audio_Buffer_1[2*i+1] = Audio_Buffer_1[2*i]; // Right channel
		cnt = cnt + 1.0f;
	}
	// Start the audio player, size is number of bytes so mult. by 2
 	BSP_AUDIO_OUT_Play((uint16_t *) Audio_Buffer_1, BUF_LEN*2);

 	// Cycle back and forth between buffers
 	while (true) {
 		// Write buffer 2
 	 	for (i=0;i<NUM_POINTS;i++) {
 			Audio_Buffer_2[2*i] = arm_cos_f32(tmp*cnt)*32000.0f; // Left channel
 			Audio_Buffer_2[2*i+1] = Audio_Buffer_2[2*i]; // Right channel
 			cnt = cnt + 1.0f;
 		}
 	 	osMessagePut(mid_bufQueue, 2, osWaitForever); 	// Send buffer number
 	 	osSemaphoreWait(SEM0_id, osWaitForever);		// Wait for reply

 	 	// Write buffer 1
 	 	for (i=0;i<NUM_POINTS;i++) {
 			Audio_Buffer_1[2*i] = arm_cos_f32(tmp*cnt)*32000.0f; // Left channel
 			Audio_Buffer_1[2*i+1] = Audio_Buffer_1[2*i]; // Right channel
 			cnt = cnt + 1.0f;
 		}
 	 	osMessagePut(mid_bufQueue, 1, osWaitForever); 	// Send buffer number
 	 	osSemaphoreWait(SEM0_id, osWaitForever);		// Wait for reply
 	}
}

/* User Callbacks: user has to implement these functions if they are needed. */
/* This function is called when the requested data has been completely transferred. */
void BSP_AUDIO_OUT_TransferComplete_CallBack (void) {
	uint16_t msg = osMessageGet(mid_bufQueue, 0).value.v;
	if (msg == 1) {
		BSP_AUDIO_OUT_ChangeBuffer((uint16_t*) Audio_Buffer_1, BUF_LEN);
	}
	else if (msg == 2) {
		BSP_AUDIO_OUT_ChangeBuffer((uint16_t*) Audio_Buffer_2, BUF_LEN);
	}
	osSemaphoreRelease(SEM0_id);
}

/* This function is called when half of the requested buffer has been transferred. */
void BSP_AUDIO_OUT_HalfTransfer_CallBack (void) {
	// pass
}

/* This function is called when an Interrupt due to transfer error or peripheral
   error occurs. */
void BSP_AUDIO_OUT_Error_CallBack(void){
	while (1) { }
}


