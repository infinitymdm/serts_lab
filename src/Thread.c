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

void Thread_1 (void const *argument);
osThreadId tid_Thread_1;
osThreadDef (Thread_1, osPriorityNormal, 1, 0);

void Init_Thread (void) {
  LED_Initialize(); // Initialize the LEDs
  UART_Init(); // Initialize the UART
  // Create threads
  tid_Thread_1 = osThreadCreate (osThread(Thread_1), NULL);
  if (!tid_Thread_1) return;
}

void Thread_1 (void const *argument) {
	usbStatus ustatus; // USB driver status variable
	uint8_t drivenum = 0; // Using U0: drive number
	char *drive_name = "U0:"; // USB drive name
	fsStatus fstatus; // file system status variable
	static FILE *f;

	LED_On(LED_Green);
	ustatus = USBH_Initialize (drivenum); // initialize the USB Host
	if (ustatus == usbOK){
		// loop until the device is OK, may be delay from Initialize
		ustatus = USBH_Device_GetStatus (drivenum); // get the status of the USB device
		while(ustatus != usbOK){
			ustatus = USBH_Device_GetStatus (drivenum); // get the status of the USB device
		}
		// initialize the drive
		fstatus = finit (drive_name);
		if (fstatus != fsOK){
			// handle the error, finit didn't work
		} // end if
		// Mount the drive
		fstatus = fmount (drive_name);
		if (fstatus != fsOK){
			// handle the error, fmount didn't work
		} // end if
		// file system and drive are good to go
		f = fopen ("Test.txt","w");// open a file on the USB device for writing
		if (f != NULL) {
			fputs("Testing an OTG USB device.\n", f); // write a string to the file
			fclose (f); // close the file
		}

	} // end if USBH_Initialize
	LED_Off(LED_Green);
}
