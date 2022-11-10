/*************************************************************************************
 * CO503 Advanced Embedded Systems
 *
 * Date			: 16-08-2016
 * Author 		: Isuru Nawinne
 * Edited by	:
 *
 * Description	: Compress and convert a BMP image file into JPEG format
 *
 *************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <io.h>
#include <unistd.h>
#include "application/jpeg_encoding.h"
#include "application/bmp_extract.h"

#define NO_OF_FILES 7
#define LED_BASE 0x0a001010 // INSERT BASE ADDRESS OF "led_out" PIO DEVICE FROM QSYS
#define OFFSET 0x00000000
#define Started_state 1
#define Processing_state 2
#define Finished_state 3
#define Error_state 4
//static uint8_t count;


void convert_picture(const char *jtag_input);


int main()
{
	char jtag_input[20];

	// Print that welcome message. Programmers looove welcome messages! ;)
	printf("CO503 JPEG Encoder \n\n");
//	count = 1;
	while(1) {
		printf("Input file: ");
		scanf("%s", jtag_input); // Ask for the input BMP file
		// Check if the filename ends in '.bmp'
		if(strcmp(&jtag_input[strlen(jtag_input) - 4], ".bmp") == 0) {
			IOWR_8DIRECT(LED_BASE,OFFSET,Started_state);

			convert_picture(jtag_input); // See function at the bottom
			IOWR_8DIRECT(LED_BASE,OFFSET,Finished_state);

		}
		printf("\n\n");
	}

	return 0;
}

/*
 * Convert a BMP picture of name <jtag_input> into JPEG.
 */
void convert_picture(const char *jtag_input)
{
	char file_name[80] = "/mnt/host/files/";
	char destination[80] = "/mnt/host/files/";
	Bmp_data pic_data;

	strcat(file_name, jtag_input);
	strcat(destination, jtag_input);

	// Replace the '.bmp' with '.jpg'
	strcpy(&destination[strlen(destination) - 3], "jpg");

	IOWR_8DIRECT(LED_BASE,OFFSET,Processing_state);
	if(bmp_extract(file_name, &pic_data) == 0) {
		// Convert to JPEG. This is where the magic happens!
		jpeg_encode(destination, pic_data.bitmap, pic_data.header->BMPHeight, pic_data.header->BMPWidth, 90);
	} else {
		IOWR_8DIRECT(LED_BASE,OFFSET,Error_state);
		// An error has happened
	}
}
