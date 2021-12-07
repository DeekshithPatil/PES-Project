/*
* cbfifo - Circular buffer implemented using statically allocated memory
* Author - Deekshith Reddy Patil, patil.deekshithreddy@colorado.edu
*
* Implemented circular wrapping of last index of Queue by using boolean AND, as mentioned in the textbook by Elecia (White)
*/

#include "cbfifo.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <MKL25Z4.h>

#define NUMBER_OF_BUFFERS		2

cb_fifo_t buffers[NUMBER_OF_BUFFERS] = {0};


/*
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity.
 *
 * Parameters:
 *   buffer   To select between Rx and Tx Buffer
 *   data      Pointer to the data
 *   nbyte    Max number of bytes to enqueue
 *
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns -1.
 */
size_t cbfifo_enqueue(buffer_t buffer, void *data, size_t nbyte)
{
	uint32_t masking_state;
	cb_fifo_t *cb_fifo = &buffers[buffer];

	if(data == NULL)
	{
		return -1;
	}


	if((cb_fifo->first == cb_fifo->last) && (cb_fifo->length == CAPACITY)) //Queue is full and cannot accomodate any enqueue operation
	{
		//This test case can be removed because of the next one
		return 0;
	}

	if(nbyte > (CAPACITY - cb_fifo->length)) //if number of bytes to be enqueued is more than that of remaining free capacity
	{
		nbyte = CAPACITY - cb_fifo->length;
	}

	masking_state = __get_PRIMASK();

	__disable_irq();

	for(int i=0;i<nbyte;i++)
	{
		cb_fifo->cbuffer[cb_fifo->last] = *(uint8_t *)data;
		cb_fifo->last = (cb_fifo->last + 1) & (CAPACITY -1); //Implemented by referring to the textbook written by Elecia
		cb_fifo->length++;
		data++;
	}

	__set_PRIMASK(masking_state);

	return nbyte;
}

/*
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 *   buffer   To select between Rx and Tx Buffer
 *   data      Destination for the dequeued data
 *   nbyte    Bytes of data requested
 *
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte.
 *
 * To further explain the behavior: If the FIFO's current length is 24
 * bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * return the 24 bytes it has, and the new FIFO length will be 0. If
 * the FIFO is empty (current length is 0 bytes), a request to dequeue
 * any number of bytes will result in a return of 0 from
 * cbfifo_dequeue.
 */
size_t cbfifo_dequeue(buffer_t buffer, void *data, size_t nbyte)
{
	uint32_t masking_state;

	cb_fifo_t *cb_fifo = &buffers[buffer];

	if(data == NULL)
	{
		return -1;
	}

	if((cb_fifo->first == cb_fifo->last) && (cb_fifo->length == 0)) //Queue is empty
	{
		return 0;
	}

	if(nbyte>=cb_fifo->length) //If requested number of dequeue bytes is greater than current length of Queue
	{
		nbyte = cb_fifo->length;
	}

	masking_state = __get_PRIMASK();

	__disable_irq();

	for(int i=0;i<nbyte;i++)
	{
		*(uint8_t *)data = cb_fifo->cbuffer[cb_fifo->first];
		data++;

		cb_fifo->first = (cb_fifo->first + 1) & (CAPACITY -1); //Implemented by referring to the textbook written by Elecia
		cb_fifo->length--;

	}

	__set_PRIMASK(masking_state);

	return nbyte;

}

/*
 * Returns the FIFO's capacity
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   The capacity, in bytes, for the FIFO
 */
size_t cbfifo_capacity(buffer_t buffer)
{
	return (size_t)CAPACITY;
}

/*
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length(buffer_t buffer)
{
	cb_fifo_t *cb_fifo = &buffers[buffer];

	return cb_fifo->length;
}
