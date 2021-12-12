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
//	int x = 0;
#ifdef DEBUG
  	test_cbfifo();
#endif

	sysclock_init();

	Init_UART0();

	printf("\nHey! How's it going\n This is a huge string test. Let's see if it works!!!. I'm so excited!!\n");

	printf("The second string is long as well! This is decoded perfectly as well!!\n");

	for(int i= 0; i< 10; i++)
	{
		printf("\nDynamic string No. = %d",i+1);
	}

	while (1)
	{

	}

  return 0 ;
}



