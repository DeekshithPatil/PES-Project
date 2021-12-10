/*
 * main.c - application entry point
 *
 * Author Deekshith Reddy Patil, patil.deekshithreddy@colorado.edu
 */
#include "sysclock.h"
#include "UART.h"
#include "cbfifo.h"
#include "syscalls.h"
#include "test_cbfifo.h"

int main(void)
{

#ifdef DEBUG
  	test_cbfifo();
#endif

	sysclock_init();

	Init_UART0();

	printf(" e9a4");

	while (1)
	{

	}

  return 0 ;
}



