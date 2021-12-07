/*
 * main.c - application entry point
 *
 * Author Deekshith Reddy Patil, patil.deekshithreddy@colorado.edu
 */
#include "sysclock.h"
#include "UART.h"
#include "cbfifo.h"
#include "syscalls.h"
#include "FSM.h"
#include "test_cbfifo.h"


int main(void)
{

  	char str[CAPACITY + 1] = "";

#ifdef DEBUG
  	test_cbfifo();
#endif

	sysclock_init();

	Init_UART0();

	printf("\r\nWelcome to BreakfastSerial!\r\n");

	while (1)
	{
		printf("\r\n? ");

		accumulate_line(str);

		process_command(str);

	}

  return 0 ;
}



