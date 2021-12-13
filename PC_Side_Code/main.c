/*
* Brief - This program is used to read data from serial terminal, and perform huffman decoding
* Author - Deekshith Reddy Patil, patil.deekshithreddy@colorado.edu
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
#define FILE_READ_COUNT_ERROR_THRESHOLD   500
#define SET_LAST_BIT_MASK                 0x01

unsigned int file_read_count = 0;

#define G_ARRAY_SIZE                       256
uint8_t g_array[G_ARRAY_SIZE] = {0};
int g_index = 0;

/*
* Brief - Used to start reading data from the serial port
*/
void start_serial_read()
{
    uint8_t read_data = 0;
    int len = 0;
    unsigned char decoded_data = 0; //holds the decoded data, after going through the huffman table and decoding the received bit stream
    uint32_t temp_data = 0;
    int read_len = 0; //Holds the number of bits currently read.
                      //Used to compare with code_bits in huff_table
    printf("****************************************\n");
    printf("Starting serial read!\nWaiting for data..\n");

    while(1)
    {
      g_index = 0;
      file_read_count = 0;

     //Read 1 byte of data from serial port and add to g_array
      do
      {
        file_read_count++; //file_read_count keeps a track of the number of times it failed to read from the serial port
        len = file_read_data(g_fd,&g_array[g_index],1); //Read one byte of data

        if(len!= NO_BYTE_AVAILABLE) //If read was Successful
        {
          g_index++;
          file_read_count = 0;
        }

      }while((g_index < G_ARRAY_SIZE) && (file_read_count < FILE_READ_COUNT_ERROR_THRESHOLD));
      //Loop as long as the number of bytes read is less thant max_size and number of unsuccessful reads is less than count

      temp_data = 0;
      read_len = 0;
      file_read_count = 0;

      //Loop Through the buffered array, byte-by-byte
      for(int front_index = 0; front_index <= g_index; front_index++)
      {

        read_data = g_array[front_index];

        //Loop 8 times (all the bits in the byte), before reading the next byte of data.
        for(int iteration = 0; iteration < 8; iteration++)
        {

          temp_data = temp_data << 1;
          //Check if the Most significant bit is a one
          if(read_data & MSB_BIT_MASK)
          {
            temp_data |= (SET_LAST_BIT_MASK); //append 1 into last bit
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
            //Iterate throughout the table
            for(int i=0; (huffman_code[i].code!=HUFF_CODE_END_SYMBOL) || (huffman_code[i].code_bits > 0);i++)
            {
              //Check if current len and code matches with any in lookup table
              if((temp_data == huffman_code[i].code) && (read_len == huffman_code[i].code_bits))
              {
                //found!!
                decoded_data = huffman_code[i].symbol;
                break;
              }
            }
            //If the decoding was Successful
            if(decoded_data != HUFF_CODE_END_SYMBOL)
            {
              //Print decoded data and reset temp_data and read_len
              setvbuf (stdout, NULL, _IONBF, 0);
              if((decoded_data != '-') && (front_index != g_index))
                printf("%c", decoded_data);
              temp_data = 0;
              read_len = 0;
            }
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
