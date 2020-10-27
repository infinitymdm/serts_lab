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

typedef struct WAVHEADER {
	unsigned char riff[4];
	uint32_t overall_size;
	unsigned char wave[4];
	unsigned char fmt_chunk_marker[4];
	uint32_t length_of_fmt;
	uint16_t format_type;				// 1-PCM, 3-IEEE float, 6-8bit A law, 7-8bit mu law
	uint16_t channels;
	uint32_t sample_rate;
	uint32_t byterate;
	uint16_t block_align;
	uint16_t bits_per_sample;
	unsigned char data_chunk_header[4];
	uint32_t data_size;
} WAVHEADER;

// pointer to the file
FILE *f;

// buffer used for audio play
int16_t Audio_Buffer[2][BUF_LEN];

// Semaphores
osSemaphoreDef (SEM0);
osSemaphoreId (SEM0_id);

// Message Queues
osMessageQId mid_bufQueue;
osMessageQDef(bufQueue, 1, uint32_t);

// Threads
void FS (void const *argument); // Thread_1 prototype
osThreadId tid_Thread_1;
osThreadDef(FS, osPriorityNormal, 1, 0);

void Init_Thread (void) {
	LED_Initialize(); // Initialize the LEDs
	UART_Init(); // Initialize the UART

	// Create semaphores
	SEM0_id = osSemaphoreCreate(osSemaphore(SEM0), 0);

	// Create queues
	mid_bufQueue = osMessageCreate(osMessageQ(bufQueue), NULL);

	// Create threads
	tid_Thread_1 = osThreadCreate(osThread(FS), NULL);
	if (!tid_Thread_1) return;

	if (!mid_bufQueue || !tid_Thread_1) return;
}

void FS (void const *argument) {
	usbStatus ustatus; // USB driver status variable
	uint8_t drivenum = 0; // Using U0: drive number
	char *drive_name = "U0:"; // USB drive name
	fsStatus fstatus; // file system status variable
	WAVHEADER header;
	int i = 0;
	static uint8_t rtrn;

	ustatus = USBH_Initialize (drivenum); // initialize the USB Host
	if (ustatus == usbOK){
		// loop until the device is OK, may be delay from Initialize
		ustatus = USBH_Device_GetStatus (drivenum); // get the status of the USB device
		while(ustatus != usbOK){
			ustatus = USBH_Device_GetStatus (drivenum); // get the status of the USB device
		}
		// initialize the drive
		fstatus = finit (drive_name);
		if (fstatus != fsOK) return;
		// Mount the drive
		fstatus = fmount (drive_name);
		if (fstatus != fsOK) return;
		// file system and drive are good to go
		f = fopen ("Test.wav","r");// open a file on the USB device
		if (f != NULL) {
			// Read the header
			fread((void *)&header, sizeof(header), 1, f);

			// Initialize the audio device
			rtrn = BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 0x46, header.sample_rate);
			if (rtrn != AUDIO_OK) return;

			fread(Audio_Buffer[i], sizeof(int16_t), BUF_LEN, f); // Read the first chunk of audio
			BSP_AUDIO_OUT_Play((uint16_t *) Audio_Buffer[i], BUF_LEN*2);
			i++;
			while (fread(Audio_Buffer[i], sizeof(int16_t), BUF_LEN, f)) {
				osMessagePut(mid_bufQueue, i+1, osWaitForever);
				osSemaphoreWait(SEM0_id, osWaitForever);
				i++;
				if (i > 1) i = 0;
			}
			fclose(f);
			BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
		} // end if file opened
	} // end if USBH_Initialize
} // end Thread


/* User Callbacks: user has to implement these functions if they are needed. */
/* This function is called when the requested data has been completely transferred. */
void BSP_AUDIO_OUT_TransferComplete_CallBack (void) {
	uint16_t msg = osMessageGet(mid_bufQueue, 0).value.v;
	if (msg == 1) {
		BSP_AUDIO_OUT_ChangeBuffer((uint16_t*) Audio_Buffer[0], BUF_LEN);
	}
	else if (msg == 2) {
		BSP_AUDIO_OUT_ChangeBuffer((uint16_t*) Audio_Buffer[1], BUF_LEN);
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
	while (true) { }
}


