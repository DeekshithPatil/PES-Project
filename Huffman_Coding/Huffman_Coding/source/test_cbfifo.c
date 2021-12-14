/*
 * test_cbfifo.c - test the cbfifo implementation
 * 
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 * 
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "test_cbfifo.h"
#include "cbfifo.h"


void test_cbfifo()
{

	size_t capacity = cbfifo_capacity(0);

	assert(capacity == 256);

	capacity = cbfifo_capacity(1);

	assert(capacity == 256);


	//Attempt to Dequeue and Enqueue when buf = NULL
	char * buf = NULL;
	int nbytes = 10;


	size_t dequeueLen = cbfifo_dequeue(Rx_Buffer,(void*)buf,nbytes);

	assert(dequeueLen == (size_t)-1);

	size_t enqueueLen = cbfifo_enqueue(Rx_Buffer,(void *)buf,nbytes);
	assert(enqueueLen == (size_t)-1);

	dequeueLen = cbfifo_dequeue(Tx_Buffer,(void*)buf,nbytes);

	assert(dequeueLen == (size_t)-1);

	enqueueLen = cbfifo_enqueue(Tx_Buffer,(void *)buf,nbytes);
	assert(enqueueLen == (size_t)-1);

	//Attempt to dequeue an empty queue
	nbytes = 5;

	uint8_t buf1[nbytes];

	dequeueLen = cbfifo_dequeue(Rx_Buffer,(void *)buf1,nbytes);

	assert(dequeueLen == 0);

	dequeueLen = cbfifo_dequeue(Tx_Buffer,(void *)buf1,nbytes);

	assert(dequeueLen == 0);

	//Enqueue and dequeue, 16 bytes of data
	int arr[4] = {1,2,3,4};

	int dequeueArr[4];

	nbytes= sizeof(arr);

	enqueueLen = cbfifo_enqueue(Rx_Buffer,(void *)arr,nbytes);

	assert(enqueueLen == nbytes);

	dequeueLen = cbfifo_dequeue(Rx_Buffer,(void *)dequeueArr,nbytes);

	assert(dequeueLen == nbytes);

	for(int i=0;i<(dequeueLen/sizeof(int));i++)
	{
	  assert(dequeueArr[i] == arr[i]);

	}

	enqueueLen = cbfifo_enqueue(Tx_Buffer,(void *)arr,nbytes);

	assert(enqueueLen == nbytes);

	dequeueLen = cbfifo_dequeue(Tx_Buffer,(void *)dequeueArr,nbytes);

	assert(dequeueLen == nbytes);

	for(int i=0;i<(dequeueLen/sizeof(int));i++)
	{
	  assert(dequeueArr[i] == arr[i]);

	}

	//Random Enqueue and Dequeue
	int arr1[10] = {1,2,3,4,5,6,7,8,9};

	int dequeueArr1[10];

	enqueueLen = cbfifo_enqueue(Rx_Buffer,arr1, 7 * sizeof(int)); //Enqueue first seven elements

	assert(enqueueLen == 7 * sizeof(int));

	dequeueLen = cbfifo_dequeue(Rx_Buffer,dequeueArr1,2 * sizeof(int)); //Dequeue 2 elements

	assert(dequeueLen == 2 * sizeof(int));
	assert(cbfifo_length(Rx_Buffer) == 5 * sizeof(int));
	assert(dequeueArr1[0] == 1);
	assert(dequeueArr1[1] == 2);

	dequeueLen = cbfifo_dequeue(Rx_Buffer,&dequeueArr1[2],3 * sizeof(int)); //Dequeue 3 elements

	assert(dequeueLen == 3 * sizeof(int));
	assert(cbfifo_length(Rx_Buffer) == 2 * sizeof(int));
	assert(dequeueArr1[2] == 3);
	assert(dequeueArr1[3] == 4);
	assert(dequeueArr1[4] == 5);

	enqueueLen = cbfifo_enqueue(Rx_Buffer,&arr1[7],3 * sizeof(int)); //Enqueue last 3 elements

	assert(enqueueLen == 3 * sizeof(int));
	assert(cbfifo_length(Rx_Buffer) == 5 * sizeof(int));

	dequeueLen = cbfifo_dequeue(Rx_Buffer,&dequeueArr1[5],5 * sizeof(int)); //Dequeue  last five elements

	assert(cbfifo_length(Rx_Buffer) == 0);

	for(int i=0;i<10;i++)
	{
	  assert(arr1[i] == dequeueArr1[i]);
	}

	enqueueLen = cbfifo_enqueue(Tx_Buffer,arr1, 7 * sizeof(int)); //Enqueue first seven elements

	assert(enqueueLen == 7 * sizeof(int));

	dequeueLen = cbfifo_dequeue(Tx_Buffer,dequeueArr1,2 * sizeof(int)); //Dequeue 2 elements

	assert(dequeueLen == 2 * sizeof(int));
	assert(cbfifo_length(Tx_Buffer) == 5 * sizeof(int));
	assert(dequeueArr1[0] == 1);
	assert(dequeueArr1[1] == 2);

	dequeueLen = cbfifo_dequeue(Tx_Buffer,&dequeueArr1[2],3 * sizeof(int)); //Dequeue 3 elements

	assert(dequeueLen == 3 * sizeof(int));
	assert(cbfifo_length(Tx_Buffer) == 2 * sizeof(int));
	assert(dequeueArr1[2] == 3);
	assert(dequeueArr1[3] == 4);
	assert(dequeueArr1[4] == 5);

	enqueueLen = cbfifo_enqueue(Tx_Buffer,&arr1[7],3 * sizeof(int)); //Enqueue last 3 elements

	assert(enqueueLen == 3 * sizeof(int));
	assert(cbfifo_length(Tx_Buffer) == 5 * sizeof(int));

	dequeueLen = cbfifo_dequeue(Tx_Buffer,&dequeueArr1[5],5 * sizeof(int)); //Dequeue  last five elements

	assert(cbfifo_length(Tx_Buffer) == 0);

	for(int i=0;i<10;i++)
	{
	  assert(arr1[i] == dequeueArr1[i]);
	}

	//Random enqueue and dequeue
	  uint8_t enqueueArr2[128] = {0};
	  uint8_t dequeueArr2[128];

	  enqueueLen = cbfifo_enqueue(Rx_Buffer,enqueueArr2,120);

	  assert(enqueueLen == 120);


	  dequeueLen = cbfifo_dequeue(Rx_Buffer,dequeueArr2,24);
	  assert(dequeueLen == 24);

	  enqueueLen = cbfifo_enqueue(Rx_Buffer,enqueueArr2,12);

	  dequeueLen = cbfifo_dequeue(Rx_Buffer,dequeueArr2,106);

	  enqueueLen = cbfifo_enqueue(Rx_Buffer,enqueueArr2,12);

	  dequeueLen = cbfifo_dequeue(Rx_Buffer,dequeueArr2,20);

	  enqueueLen = cbfifo_enqueue(Rx_Buffer,enqueueArr2,127);

	  enqueueLen = cbfifo_enqueue(Rx_Buffer,enqueueArr2,120);

	  assert(enqueueLen == 120);

	  uint8_t enqueueArr3[128] = {0};
	  uint8_t dequeueArr3[128];
	  enqueueLen = cbfifo_enqueue(Tx_Buffer,enqueueArr2,120);

	  assert(enqueueLen == 120);
	  dequeueLen = cbfifo_dequeue(Tx_Buffer,dequeueArr3,24);
	  assert(dequeueLen == 24);

	  enqueueLen = cbfifo_enqueue(Tx_Buffer,enqueueArr3,12);

	  dequeueLen = cbfifo_dequeue(Tx_Buffer,dequeueArr3,106);

	  enqueueLen = cbfifo_enqueue(Tx_Buffer,enqueueArr3,12);

	  dequeueLen = cbfifo_dequeue(Tx_Buffer,dequeueArr3,20);

	  enqueueLen = cbfifo_enqueue(Tx_Buffer,enqueueArr3,127);

	  //Reset
	  while(cbfifo_length(Rx_Buffer))
	  {
		  dequeueLen = cbfifo_dequeue(Rx_Buffer,dequeueArr2,1);
	  }

	  while(cbfifo_length(Tx_Buffer))
	  {
		  dequeueLen = cbfifo_dequeue(Tx_Buffer,dequeueArr3,1);
	  }

	  capacity = capacity + 1;
	  enqueueLen = enqueueLen + 1;
}
