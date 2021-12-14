/*
 * Delay.c
 *
 *  Created on: Dec 12, 2021
 *      Author: deekshith
 */

#include "Delay.h"

/*
 * Used to generate delay in millilseconds
 */

void delay(uint32_t millis)
{
	uint32_t MilliSecondsCounter;
	while(millis--)
	{
		MilliSecondsCounter= 5200;
		while(MilliSecondsCounter-- !=0)
		{
			__asm volatile("NOP");
		}
	}
}
