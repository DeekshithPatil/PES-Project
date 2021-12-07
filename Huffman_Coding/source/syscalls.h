/*
 * syscalls.h
 *
 *  Created on: Nov 7, 2021
 *      Author: Deekshith Reddy Patil, patil.deekshithreddy@colorado.edu
 */


#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include "sysclock.h"
#include "UART.h"
#include "cbfifo.h"

/*
 * Brief - Writes the specified bytes to either stdout (handle = 1) or stderr (handle = 2). For our purposes
* you may ignore the handle argument, since both are tied to the serial output.
* Return:  -1 on erroror 0 on success.
 */
int __sys_write(int handle, char *buf, int size);

/*
 * Brief - Reads one character from the serial connection and returns it.
 * Return: -1 if no data is available to be read.
 */
int __sys_readc(void);

#endif /* SYSCALLS_H_ */
