/*
 * huffman.c
 *
 * Brief - Performs necessary encoding and decoding based on the huffman codes for each symbol present in huffman.h
 *
 *  Created on: Dec 8, 2021
 *      Author: Deekshith Reddy Patil, patil.deekshithreddy@colorado.edu
 */

#include "huffman.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#define MASK(x)					((1U << x) - 1)

static int min(int a , int b);

//Refer huffman.h for function description
int huffman_encode(char *message, uint8_t *buffer, size_t nbytes)
{
	int buf_idx = 0; //Current write position encoded
	int bits_written = 0; // Number of bits already written at buf_idx
	int hc_idx;	//Huffman code index for the current symbol

	memset(buffer,0,nbytes);

	for(char *p = message; *p!= '\0'; p++)
	{
		for(hc_idx = 0; huffman_code[hc_idx].symbol != *p; hc_idx++)
			assert(huffman_code[hc_idx].symbol != '\0');

		uint32_t code = huffman_code[hc_idx].code;
		int code_bits = huffman_code[hc_idx].code_bits;

		while(code_bits > 0)
		{
			//this_write: the number of bits to encode in this step
			int this_write = min(8-bits_written, code_bits);
			assert(bits_written + this_write <= 8);

			//read_shift: position within code to get the bits from
			//tmp: bits to write, shifted down to lsb
			int read_shift = code_bits - this_write;
			uint32_t tmp = (code >> read_shift) & MASK(this_write);

			//buff[buf_idx]: buffer we're writing into
			//write_shift: shift to fit into this byte of the buffer
			int write_shift = 8 - bits_written - this_write;
			buffer[buf_idx] |= tmp << write_shift;

			bits_written += this_write;
			code_bits -= this_write;

			if(bits_written == 8)
			{
				bits_written = 0;
				buf_idx++;
				assert(buf_idx < nbytes);
			}
		}
	}

	return buf_idx+1;

}

static int min(int a , int b)
{
	if(a < b)
		return a;

	return b;
}
