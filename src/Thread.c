
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
void delay(void);
void Thread_1 (void const *argument);           // thread function 1
void Thread_2 (void const *argument);           // thread function 2
osThreadId tid_Thread_1;                      	// thread id 1
osThreadId tid_Thread_2;                      	// thread id 2
osThreadDef (Thread_1, osPriorityNormal, 1, 0); // thread object 1
osThreadDef (Thread_2, osPriorityAboveNormal, 1, 0); // thread object 2

#define MSGQUEUE_OBJECTS    2                 	// number of Message Queue Objects

osMessageQId mid_MsgQueue;                     	// message queue id
osMessageQDef (MsgQueue, MSGQUEUE_OBJECTS, int32_t); // message queue object

// FUNCTION DEFINITIONS
void delay(void){
  int32_t j;
  for(j=0;j<20000000;j++){};
}

int Init_Thread (void) {
  LED_Initialize(); // Initialize the LEDs
  UART_Init(); // Initialize the UART
  mid_MsgQueue = osMessageCreate (osMessageQ(MsgQueue), NULL);  // create msg queue
  if (!mid_MsgQueue) {
	  ; // Message Queue object not created, handle failure
  }
  tid_Thread_1 = osThreadCreate (osThread(Thread_1), NULL);
  if (!tid_Thread_1) return(-1);
  tid_Thread_2 = osThreadCreate (osThread(Thread_2), NULL);
  if (!tid_Thread_2) return(-1);

  return(0);
}

void Thread_1 (void const *argument) {
  int32_t tx_msg_data=0; // data to send
  while (1) {
    LED_On(0);
    delay();
    osMessagePut (mid_MsgQueue, tx_msg_data, osWaitForever); // Send Message
    tx_msg_data++; // Make a new value to send to the other thread
    LED_Off(0);
    delay();
  }
}

void Thread_2 (void const *argument) {
  osEvent evt; // receive object
  int32_t rx_msg_data; // received data
  while (1) {
    LED_On(1);
    delay();
    evt = osMessageGet (mid_MsgQueue, osWaitForever);        // wait for message
    if (evt.status == osEventMessage) { // check for valid message
    rx_msg_data = evt.value.v;
    LED_Off(1);
    delay();
   }
  }
}

