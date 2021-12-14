/*
 * UART.c
 *
 *  Created on: Nov 3, 2021
 *      Author: Deekshith Reddy Patil, patil.deekshithreddy@colorado.edu
 *  Implementation Credits: Alexader Dean, textbook
 */

#include "UART.h"
#include "cbfifo.h"


#define USE_UART_INTERRUPTS 	(0) // 0 for polled UART communications, 1 for interrupt-driven
#define UART_OVERSAMPLE_RATE 	(16)

#define SYS_CLOCK				(24e6)
#define	BAUD_RATE				115200
#define STOP_BITS_NO			2
#define	NO_PARITY				0

//Refer UART.h for description
void Init_UART0()
{
	uint32_t baud_rate = BAUD_RATE;
	uint16_t sbr;
	uint8_t temp;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 24 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	SIM->SOPT2 &= ~(SIM_SOPT2_PLLFLLSEL_MASK); //Select FLL

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	// Disable interrupts for RX active edge and LIN break detect, select two stop bits
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_BITS_NO-1) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(NO_PARITY);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Try it a different way
	UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);


	// Enable interrupts. Listing 8.11 on p. 234

	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);


	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	// Clear the UART RDRF flag
	temp = UART0->D;
	temp = temp + 1; //Just setting it to arbitrary value, in order to avoid warning raised by compiler
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

}

/*
 * Brief - UART IRQ Handler
 */
void UART0_IRQHandler(void) {
	uint8_t ch;
	size_t temp;

	//Process Rx
	if (UART0->S1 & UART0_S1_RDRF_MASK)
	{

		ch = UART0->D;

		if(cbfifo_length(Rx_Buffer) != cbfifo_capacity(Rx_Buffer)) //Check if Rx buffer is not full
		{
			temp = cbfifo_enqueue(Rx_Buffer,&ch,1);
		}
		else
		{
			// Discard the byte
		}
	}

	//Process Tx
	if ( (UART0->C2 & UART0_C2_TIE_MASK) && // transmitter interrupt enabled
			(UART0->S1 & UART0_S1_TDRE_MASK) )
	{ // tx buffer empty
		// can send another character
		if(cbfifo_length(Tx_Buffer) != 0) //If there is something to transmit in the transmit buffer
		{
			temp = cbfifo_dequeue(Tx_Buffer,&ch,1);
			UART0->D = ch;
		}
		else
		{
			// queue is empty so disable transmitter interrupt
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}

	temp = temp + 1; //To avoid compiler warninng
}
