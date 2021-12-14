/*
 * syscalls.c
 *
 *  Created on: Nov 7, 2021
 *      Author: Deekshith Reddy Patil. patil.deekshithreddy@colorado.edu
 */


#include "syscalls.h"
#include "Delay.h"
#include <stdint.h>

extern int huffman_encode(char *message, uint8_t *buffer, size_t nbytes);

#define ENCODE_BUFFER_SIZE					1024
uint8_t encode_buffer[ENCODE_BUFFER_SIZE] = {0};


int __sys_readc(void)
{
	int c;
	size_t temp;

	while(cbfifo_length(Rx_Buffer) == 0){} //Wait as long as receive buffer is empty

	temp = cbfifo_dequeue(Rx_Buffer,&c,1);

	if(temp != 1)
		return -1;
	temp = temp + 1; // To avoid compiler warning
	return c;

}

int __sys_write(int handle, char *buf, int size)
{
	size_t temp;


	int len = huffman_encode(buf, encode_buffer, ENCODE_BUFFER_SIZE);

	//Blocking call. Wait as long as Tx_buffer is not empty
	while(cbfifo_length(Tx_Buffer) != 0){delay(2);}

	//If TxQueue is not full, enqueue the bytes
	if(cbfifo_length(Tx_Buffer) != cbfifo_capacity(Tx_Buffer))
	{
		temp = cbfifo_enqueue(Tx_Buffer,encode_buffer,len);

		// start transmitter if it isn't already running
		if (!(UART0->C2 & UART0_C2_TIE_MASK))
		{
			UART0->C2 |= UART0_C2_TIE(1);
		}

		if(temp == size)
			return 0;
	}


	return -1;
}
