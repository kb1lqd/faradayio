/*
 * main.h
 *
 *  Created on: Jan 8, 2018
 *      Author: brent
 */

#ifndef MAIN_H_
#define MAIN_H_

int ConnectDevice(void);
int SetCbusDirection(unsigned char bitmask);
int EnableCBUS0(void);
int EnableCBUS1(void);
int DisableCBUS0(void);
int DisableCBUS1(void);
int EnableFaradayBslMode(struct ftdi_context *ftdi);
int DisableFaradayBslMode(struct ftdi_context *ftdi);

#endif /* MAIN_H_ */
