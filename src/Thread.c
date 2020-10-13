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

#define Show_Files_char "1"

enum commands {
	ListFiles,
	SendComplete,
	SendFiles,
};

enum state {
	NoState,
	IdleState,
	ListState,
};

//////////////////////////////////////////////////////////
// Control thread
void Control (void const *argument); // thread function
osThreadId tid_Control; // thread id
osThreadDef (Control, osPriorityNormal, 1, 0); // thread object

void FS_Thread (void const *argument);
osThreadId tid_FS_Thread;
osThreadDef (FS_Thread, osPriorityNormal, 1, 0);

// UART receive thread
void Rx_Command (void const *argument);  // thread function
osThreadId tid_RX_Command;  // thread id
osThreadDef (Rx_Command, osPriorityNormal, 1, 0); // thread object

// Command queue from Rx_Command to Control
osMessageQId mid_CMDQueue; // message queue for commands to Thread
osMessageQDef (CMDQueue, 1, uint32_t); // message queue object

// Command queue from Control to FS_Thread
osMessageQId comman_FSQueue;
osMessageQDef (FSQueue, 1, uint32_t);

void Process_Event(uint16_t event){
	static uint16_t   Current_State = NoState; // Current state of the SM
	switch(Current_State) {
    	case NoState:
    		// Next State
    		Current_State = IdleState;
    		// Exit actions
    		// Transition actions
    		// IdleState entry actions
    		LED_On(LED_Red);
    		break;
    	case IdleState:
    		if (event == ListFiles) {
    			// Next State
    			Current_State = ListState;
    			// Exit Actions
    			LED_Off(LED_Red);
    			// Transition actions
    			// ListState Entry actions
    			LED_On(LED_Blue);
    			osMessagePut(comman_FSQueue, SendFiles, osWaitForever);
    		}
    		break;
    	case ListState:
    		if (event == SendComplete) {
    			// Next State
    			Current_State = IdleState;
    			// Exit Actions
    			LED_Off(LED_Blue);
    			// Transition Actions
    			// IdleState Entry actions
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
	comman_FSQueue = osMessageCreate (osMessageQ(FSQueue), NULL);
	if (!comman_FSQueue) return;
	// Create threads
	tid_Control = osThreadCreate (osThread(Control), NULL);
	if (!tid_Control) return;
	tid_FS_Thread = osThreadCreate(osThread(FS_Thread), NULL);
	if (!tid_FS_Thread) return;
	tid_RX_Command = osThreadCreate (osThread(Rx_Command), NULL);
	if (!tid_RX_Command) return;
}

void Control (void const *argument) {
	osEvent evt;
	Process_Event(0);
	while (true) {
		evt = osMessageGet(mid_CMDQueue, osWaitForever);
		if (evt.status == osEventMessage) {
			Process_Event(evt.value.v);
		}
	}
}

void FS_Thread (void const *argument) {
	osEvent evt;
	usbStatus ustatus;
	uint8_t drivenum = 0;
	char *drive_name = "U0";
	fsStatus fstatus;
	fsFileInfo info;
	ustatus = USBH_Initialize(drivenum);
	if (ustatus == usbOK) {
		ustatus = USBH_Device_GetStatus(drivenum);
		while (ustatus != usbOK) {
			ustatus = USBH_Device_GetStatus(drivenum);
		}
		fstatus = finit(drive_name);
		if (fstatus != fsOK) {
			return;
		}
		fstatus = fmount(drive_name);
		if (fstatus != fsOK) {
			return;
		}
		while (true) {
			evt = osMessageGet(comman_FSQueue, osWaitForever);
			if (evt.status == osEventMessage && evt.value.v == SendFiles) {
				char *StartFileList_msg = "2\n";
				char *EndFileList_msg = "3\n";
				char *file_buffer;
				UART_send(StartFileList_msg, 2);
				info.fileID = 0;
				while (ffind("*.txt", &info) == fsOK) {
					file_buffer = info.name;
					UART_send(file_buffer, strlen(file_buffer));
					UART_send("\n", 1);
				}
				UART_send(EndFileList_msg, 2);
				osMessagePut(mid_CMDQueue, SendComplete, osWaitForever);
			}
		}
	}
}

void Rx_Command (void const *argument) {
	char rx_char[2] = {0,0};
	while (true) {
		UART_receive(rx_char, 1);
		if (!strcmp(rx_char, Show_Files_char)) {
			osMessagePut(mid_CMDQueue, ListFiles, osWaitForever);
		}
	}
}
