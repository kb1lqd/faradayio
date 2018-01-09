/*
    Modified version of https://github.com/BuLogics/libftdi/blob/master/examples/bitbang_cbus.c
    BITMASK
    CBUS Bits
    3210 3210
    xxxx xxxx
    |    |------ Output Control 0->LO, 1->HI
    |----------- Input/Output   0->Input, 1->Output
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ftdi.h>
#include "main.h"


#define CBUS0_DIR 0x10
#define CBUS1_DIR 0x20
#define CBUS2_DIR 0x40
#define CBUS3_DIR 0x80
#define CBUS0_STATE 0x1
#define CBUS1_STATE 0x2
#define CBUS2_STATE 0x4
#define CBUS3_STATE 0x8

#define delaytime 1 //second(s)

unsigned long vendorid = 0x0403; // FTDI
unsigned long productid = 0x6015; //FT230x
unsigned  char cbusbitmask = 0x00;


int main( int argc, char *argv[] )
{
	printf("argc: %d\n",argc);
	printf("argv: %s %d\n",argv[0], atoi(argv[1]));
	if(argc != 2){
		//Error correct operation requires 1 argument
		fprintf(stderr, "ERROR: Correct operation requires 1 argument [0 = Disable BSL | 1 = Enable BSL]!");
	}
	else{
		// Setup and connect to device
		struct ftdi_context *ftdi;
		int f;

		if ((ftdi = ftdi_new()) == 0)
		{
			fprintf(stderr, "ftdi_new failed\n");
			return EXIT_FAILURE;
		}

		printf("Vendor ID: %#010x\n",vendorid);
		printf("Product ID: %#010x\n",productid);

		f = ftdi_usb_open(ftdi, 0x0403, 0x6015);
		if (f < 0 && f != -5)
		{
			fprintf(stderr, "unable to open ftdi device: %d (%s)\n", f, ftdi_get_error_string(ftdi));
			ftdi_free(ftdi);
			exit(-1);
		}
		printf("Successful Connection. FTDI open returned: %d\n",f);

		// Set CBUS bitmask direction nibble
		SetCbusDirection(0x03); // 0x03 = 0011b which is bitshifted up to 00110011b

		// Perform Faraday CC430 BSL TEST/RST toggle initialization routine
		unsigned char userinputmode = atoi(argv[1]);
		if(userinputmode == 1){
			// Enable BSL Mode
			printf("MODE = Enable BSL");
			EnableFaradayBslMode(ftdi);

			//Set nominal mode
			SetCbusDirection(0x00);
			ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
		}

		else if(userinputmode == 0){
			// Disable BSL Mode
			printf("MODE = Disable BSL");
			DisableFaradayBslMode(ftdi);

			//Set nominal mode
			SetCbusDirection(0x00);
			ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
			}

		else{
			fprintf(stderr, "ERROR: Incorrect argument value of %d [0 = Disable BSL | 1 = Enable BSL]!", argv[2]);
		}


		// Disable bitbang mode and disconnect from device
		printf("disabling bitbang mode\n");
		ftdi_disable_bitbang(ftdi);
		ftdi_usb_close(ftdi);
		ftdi_free(ftdi);
	}

    return 0;
}

int ConnectDevice(void){

	// Return
	return 0;
}

int SetCbusDirection(unsigned char bitmask){
	//Accepts the nibble that is bitshifted  up to determine  if a CBUS port is input or output.
	cbusbitmask = (bitmask<<4);
	printf("CBUS Direction Bitmask = 0x%02x\n", cbusbitmask);

	// Return
	return 0;
}

int EnableCBUS0(void){
	// Actuate CBUS 0 state HIGH
	cbusbitmask |= CBUS0_STATE;
	printf("ENABLECBUS0: CBUS Bitmask = 0x%02x\n", cbusbitmask);

	//Return
	return 0;
}

int EnableCBUS1(void){
	// Actuate CBUS 1 state HIGH
	cbusbitmask |= CBUS1_STATE;
	printf("ENABLECBUS1: CBUS Bitmask = 0x%02x\n", cbusbitmask);

	//Return
	return 0;
}

int DisableCBUS0(void){
	// Actuate CBUS 0 state LOW
	cbusbitmask &= ~CBUS0_STATE;
	printf("DISABLECBUS0: CBUS Bitmask = 0x%02x\n", cbusbitmask);

	//Return
	return 0;
}

int DisableCBUS1(void){
	// Actuate CBUS 1 state LOW
	cbusbitmask &= ~CBUS0_STATE;
	printf("DISABLECBUS0: CBUS Bitmask = 0x%02x\n", cbusbitmask);

	//Return
	return 0;
}

int EnableFaradayBslMode(struct ftdi_context *ftdi){
	// Toggles CBUS pins to place CC430 in BSL mode
	// CBUS Assignments:
	//      RST = CBUS0
	//      TEST = CBUS1

	// Initial delay
	printf("Enabling Faraday BSL Mode!\n");
	sleep(delaytime);

	//RESET LOW
	DisableCBUS0();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

	//TEST HIGH
	EnableCBUS1();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

	//TEST LOW
	DisableCBUS1();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

	//TEST HIGH
	EnableCBUS1();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

	//RESET HIGH
	EnableCBUS0();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

	//TEST LOW
	DisableCBUS1();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

	// Return
	return 0;
}

int DisableFaradayBslMode(struct ftdi_context *ftdi){
	//Toggle CBUS to leave BSL mode per Errata (Reset won't work)
	// CBUS Assignments:
	//      RST = CBUS0
	//      TEST = CBUS1

	//RESET HIGH TEST LOW
	EnableCBUS0();
	DisableCBUS1();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

	//RESET HIGH TEST HIGH
	EnableCBUS0();
	EnableCBUS1();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

	//RESET HIGH TEST LOW
	EnableCBUS0();
	DisableCBUS1();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

	//RESET HIGH TEST HIGH
	EnableCBUS0();
	EnableCBUS1();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

	//RESET HIGH TEST LOW
	EnableCBUS0();
	DisableCBUS1();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

	//RESET LOW TEST LOW
	DisableCBUS0();
	DisableCBUS1();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

	//RESET HIGH TEST LOW
	EnableCBUS0();
	DisableCBUS1();
	ftdi_set_bitmode(ftdi, cbusbitmask, BITMODE_CBUS);
	sleep(delaytime);

}
