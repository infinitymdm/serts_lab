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
#include <stdio.h>

#define LED_Green   0
#define LED_Orange  1
#define LED_Red     2
#define LED_Blue    3

// Commands to Blue_Blink_Thread
enum Blue_Blink_Commands{
  Blue_Blink_Enable,
  Blue_Blink_Disable
};

////////////////////////////////////////////////
// State Machine definitions
enum state{
  NoState,
  State1,
  State2,
  State3
};

enum Triggers{
  Trigger1,
  Trigger2,
  Trigger3
};


// Receive characters from the VB GUI
#define Trigger1_char "1"
#define Trigger2_char "2"
#define Trigger3_char "3"

//////////////////////////////////////////////////////////
void Control (void const *argument); // thread function
osThreadId tid_Control; // thread id
osThreadDef (Control, osPriorityNormal, 1, 0); // thread object

// Command queue from Rx_Command to Controller
osMessageQId mid_CMDQueue; // message queue for commands to Thread
osMessageQDef (CMDQueue, 1, uint32_t); // message queue object

// Command queue from Controller Blue_Blink_Thread
osMessageQId mid_Blue_Blink_Queue; // message queue for commands to Thread
osMessageQDef (Blue_Blink_Queue, 1, uint32_t); // message queue object

// UART receive thread
void Rx_Command (void const *argument);  // thread function
osThreadId tid_RX_Command;  // thread id
osThreadDef (Rx_Command, osPriorityNormal, 1, 0); // thread object

// Blue Blink thread
void Blue_Blink (void const *argument);                             // thread function
osThreadId tid_Blue_Blink;                                          // thread id
osThreadDef (Blue_Blink, osPriorityNormal, 1, 0);                   // thread object

void Process_Event(uint16_t event){
  static uint16_t   Current_State = NoState; // Current state of the SM
  switch(Current_State){
    case NoState:
      // Next State
      Current_State = State1;
      // Exit actions
      // Transition actions
      // State1 entry actions
      LED_On(LED_Red);

      break;
    case State1:
      if(event == Trigger1){
        Current_State = State2;
        // Exit actions
        LED_Off(LED_Red);
        // Transition actions
        // State2 entry actions
        LED_On(LED_Green);
      }
      break;
    case State2:
      if(event == Trigger2){
        Current_State = State1;
        // Exit actions
        LED_Off(LED_Green);
        // Transition actions
        // State1 entry actions
        LED_On(LED_Red);
      }
      if(event == Trigger3){
        Current_State = State3;
        // Exit actions
        LED_Off(LED_Green);
        // Transition actions
        // State3 entry actions
        osMessagePut (mid_Blue_Blink_Queue, Blue_Blink_Enable, osWaitForever);
      }
      break;
    case State3:
      if(event == Trigger2){
        Current_State = State1;
        // Exit actions
        osMessagePut (mid_Blue_Blink_Queue, Blue_Blink_Disable, osWaitForever);
        // Transition actions
        // State1 entry actions
        LED_On(LED_Red);
      }
      break;
    default:
      break;
  } // end case(Current_State)
} // Process_Event


void Init_Thread (void) {
  LED_Initialize(); // Initialize the LEDs
  UART_Init(); // Initialize the UART
  // Create queues
  mid_CMDQueue = osMessageCreate (osMessageQ(CMDQueue), NULL);  // create msg queue
  if (!mid_CMDQueue) return; // Message Queue object not created, handle failure
  mid_Blue_Blink_Queue = osMessageCreate (osMessageQ(Blue_Blink_Queue), NULL);  // create msg queue
  if (!mid_Blue_Blink_Queue) return; // Message Queue object not created, handle failure
  // Create threads
  tid_RX_Command = osThreadCreate (osThread(Rx_Command), NULL);
  if (!tid_RX_Command) return;
  tid_Control = osThreadCreate (osThread(Control), NULL);
  if (!tid_Control) return;
  tid_Blue_Blink = osThreadCreate (osThread(Blue_Blink), NULL);
  if (!tid_Blue_Blink) return;
  }

// Thread function
void Control(void const *arg){
  osEvent evt; // Receive message object
  Process_Event(0); // Initialize the State Machine
   while(1){
    evt = osMessageGet (mid_CMDQueue, osWaitForever); // receive command
      if (evt.status == osEventMessage) { // check for valid message
      Process_Event(evt.value.v); // Process event
    }
   }
}

void Rx_Command (void const *argument){
   char rx_char[2]={0,0};
   while(1){
      UART_receive(rx_char, 1); // Wait for command from PC GUI
    // Check for the type of character received
      if(!strcmp(rx_char,Trigger1_char)){
         // Trigger1 received
         osMessagePut (mid_CMDQueue, Trigger1, osWaitForever);
      } else if (!strcmp(rx_char,Trigger2_char)){
        // Trigger2 received
         osMessagePut (mid_CMDQueue, Trigger2, osWaitForever);
      } else if (!strcmp(rx_char,Trigger3_char)){
        // Trigger3 received
         osMessagePut (mid_CMDQueue, Trigger3, osWaitForever);
      } // end if
   }
} // end Rx_Command

void Blue_Blink(void const *arg){
  osEvent evt; // Receive message object
  int16_t blink = 0; // 0 - disabled, 1 - enabled

  while(1){
    evt = osMessageGet (mid_Blue_Blink_Queue, osWaitForever); // wait for message
	if (evt.status == osEventMessage) { // check for valid message
      if( evt.value.v == Blue_Blink_Enable){
        // Blue Blink enabled
        blink = 1; // enabled
        while(blink){
          // Blink the Blue LED
          LED_On(LED_Blue);
          osDelay(1000);
          LED_Off(LED_Blue);
          osDelay(1000);
          // Check for message without any delay (no osWaitForever)
          evt = osMessageGet (mid_Blue_Blink_Queue, 0); // check queue
          if (evt.status == osEventMessage) { // check for valid message
						// valid message received
            if(evt.value.v == Blue_Blink_Disable){
							// Disable message received
              blink = 0; // Disable
            }
          } // if (evt.status == osEventMessage)
        } // while(blink)
      } // if( evt.value.v == Blue_Blink_Enable)
    } // if (evt.status == osEventMessage)
  } // while(1)
} // Blue_Blink_Thread
