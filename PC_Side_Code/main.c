/*
* Implementation credits: https://www.youtube.com/watch?v=tUJ-wTafQLA, https://github.com/sckulkarni246/ke-rpi-samples/blob/main/uart-c-termios/uart_loopback.c
*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "serial.h"
#include "huffman.h"

#define NO_BYTE_AVAILABLE                 -1
#define MSB_BIT_MASK                      0x80

/*
* Brief - Used to start reading data from the serial port
*/
void start_serial_read()
{
    uint8_t read_data = 0;
    int len = 0;
    unsigned char decoded_data = 0; //holds the decoded data, after going through the huffman table and decoding the received bit stream
    uint32_t temp_data = 0;
    int read_len=0;
    printf("Starting serial read!\nWaiting for data..\n");

    while(1)
    {
      //Reset temp_data and read_len
      temp_data = 0;
      read_len = 0;
      //Read 1 byte of data from serial port
      do
      {
        len = file_read_data(g_fd,&read_data,1);
      } while(len == NO_BYTE_AVAILABLE); //Wait as long as a byte is availabe

      //Loop 8 times, before reading the next byte of data.
      for(int iteration = 0; iteration < 8; iteration++)
      {
        temp_data = temp_data << 1;
        //Check if the Most significant bit is a one
        if(read_data & MSB_BIT_MASK)
        {
          // unsigned int shift_val = read_len;
          temp_data |= (0x01); //append 1 into appropriate bit number of temp_data
        }

        read_len++;
        read_data = read_data << 1;

       //Check if read_len is greater than HUFF_CODE_MAX_LENGTH
        if(read_len > HUFF_CODE_MAX_LENGTH)
        {
          //Reset temp_data and read_len
          temp_data = 0;
          read_len = 0;
        }

        //If read_len lies in the range of min and max length of huff codes
        else if(read_len >= HUFF_CODE_MIN_LENGTH && read_len <= HUFF_CODE_MAX_LENGTH)
        {
          decoded_data = HUFF_CODE_END_SYMBOL;
          // printf("Code = %x, Code_bits = %d\n",temp_data,read_len);
          for(int i=0; huffman_code[i].code!=HUFF_CODE_END_SYMBOL;i++) //Iterate throughout the table
          {
            if((temp_data == huffman_code[i].code) && (read_len == huffman_code[i].code_bits)) //Check if current len and code matches with any in lookup table
            {
              decoded_data = huffman_code[i].symbol;
              break;
            }
          }
          //If the decoding was Successful
          if(decoded_data != HUFF_CODE_END_SYMBOL)
          {
            // setvbuf (stdout, NULL, _IONBF, 0);
            printf("%c\n", decoded_data);
            temp_data = 0;
            read_len = 0;
          }
        }

      }
    }
}

int main(int argc, char *argv[])
{
    printf("Opening port: %s\n",argv[1]);

    assert(argc == TOTAL_ACCEPTABLE_TERMINAL_COMMANDS);

    open_serial_port(argv[1]);

    printf("Successful\n");

    configure_serial_port();

    start_serial_read();

    return 0;
}
