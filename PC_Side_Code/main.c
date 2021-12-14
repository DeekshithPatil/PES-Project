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
#include <signal.h>
#include <assert.h>

#include "serial.h"
#include "huffman.h"

#define NO_BYTE_AVAILABLE                 -1
#define MSB_BIT_MASK                      0x80
#define FILE_READ_COUNT_ERROR_THRESHOLD   500
#define SET_LAST_BIT_MASK                 0x01

#define BUFF_SIZE                         256
#define DECODE_BUFF_SIZE                  512

int g_bits_received = 0;
int g_bits_decoded = 0;

int get_buffer(uint8_t *buff);
unsigned char find_huffman_symbol(uint32_t code, int code_bits);
int huffman_decode(uint8_t *buff, int buff_index, unsigned char * decode_buffer);
void start_serial_read();
void test_huffman();
void read_and_test(char * test_str);

/*
* Brief - This function is used as an execption handler for Ctrl + C
*/
void int_handler(int sug)
{

  //Print compression ratio
  printf("\n\n********** Compression Statistics ****************\n");
  printf("\nTotal bits received = %d\nTotal bits decoded = %d\n",g_bits_received,g_bits_decoded);

  double compression_ratio = ((double)(g_bits_decoded - g_bits_received) * 100) / g_bits_decoded;

  printf("Achieved compression ratio: %f%% \n",  compression_ratio);
  printf("\n****************************************************\n");

  //Gracefully end program
  exit(0);
}

/*
* Brief - Application entry point
*/
int main(int argc, char *argv[])
{
  printf("Opening port: %s\n",argv[1]);

  assert(argc == TOTAL_ACCEPTABLE_TERMINAL_COMMANDS);

  open_serial_port(argv[1]);

  printf("Successful\n");

  configure_serial_port();

  test_huffman();

  start_serial_read();

  return 0;
}

/*
* Brief - This function is used to get the a buffer of bytes from the serial port
*       - Returns the number of bytes read
*/
int get_buffer(uint8_t *buff)
{
  int buff_index = 0;
  int len = 0;
  unsigned int file_read_count = 0;

  signal(SIGINT,int_handler);

  //Read 1 byte of data from serial port and add to buff
   do
   {
     file_read_count++; //file_read_count keeps a track of the number of times it failed to read from the serial port
     len = file_read_data(g_fd,&buff[buff_index],1); //Read one byte of data

     if(len!= NO_BYTE_AVAILABLE) //If read was Successful
     {
       buff_index++;
       file_read_count = 0;
     }

   }while((buff_index < BUFF_SIZE) && (file_read_count < FILE_READ_COUNT_ERROR_THRESHOLD));
   //Loop as long as the number of bytes read is less thant max_size and number of unsuccessful reads is less than count

   return buff_index;
}

/*
* Brief - Used to find a huffman code fromt the table, given the "code" and "code_bits" of the find_huffman_symbol
*       - Returns the decoded character (or symbol) if match was found, else 'HUFF_CODE_END_SYMBOL' is returned
*/
unsigned char find_huffman_symbol(uint32_t code, int code_bits)
{
  //Iterate throughout the table
  for(int i=0; (huffman_code[i].code!=HUFF_CODE_END_SYMBOL) || (huffman_code[i].code_bits > 0);i++)
  {
    //Check if current len and code matches with any in lookup table
    if((code == huffman_code[i].code) && (code_bits == huffman_code[i].code_bits))
    {
      //found!!
      return(huffman_code[i].symbol);
    }
  }

  return HUFF_CODE_END_SYMBOL;
}

/*
* Brief - This functon performs huffman decoding on a buffer ('buff') of length 'buf_index'
*       - The decoded bytes are printed stored in decode_buff
*       - Returns the length of decode_buff
*/
int huffman_decode(uint8_t *buff, int buff_index, unsigned char * decode_buff)
{
  uint8_t read_data = 0;
  uint32_t temp_data = 0;
  int read_len = 0; //Holds the number of bits currently read.
                    //Used to compare with code_bits in huff_table

  int decode_index = 0;

  unsigned char decoded_data = 0; //holds the decoded data, after going through the huffman table and decoding the received bit stream

  //Loop Through the buffered array, byte-by-byte
  for(int front_index = 0; front_index <= buff_index; front_index++)
  {
    read_data = buff[front_index];

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
        decoded_data = find_huffman_symbol(temp_data,read_len);

        //If the decoding was Successful
        if(decoded_data != HUFF_CODE_END_SYMBOL)
        {
          //Print decoded data and reset temp_data and read_len
         setvbuf (stdout, NULL, _IONBF, 0);
          //Do not print decoded data if decoded is '-' and it's the last byte in buffer
          if((decoded_data != '-') && (front_index != buff_index))
          {

            decode_buff[decode_index++] = decoded_data;
            //Increment the total number of bits recieved by the code_bits of the decoded symbol
            g_bits_received += read_len;
            //Increment the total number of bits decoded by 8 (because each ascii character is 1 byte long)
            g_bits_decoded +=8;
          }

          temp_data = 0;
          read_len = 0;
        }
      }

    }
  }

  decode_buff[decode_index] = '\0';
  return decode_index;
}

/*
* Brief - Used to start reading data from the serial port
*/
void start_serial_read()
{
  unsigned char decode_buff[DECODE_BUFF_SIZE];
  uint8_t buff[BUFF_SIZE];
  int buff_index = 0;
  int decode_len = 0;

  printf("\n******************************************************\n");
  printf("Starting serial read!\nWaiting for data..\n");

  while(1)
  {
    //get a buffer of data and then start decoding on it
    do
    {
      buff_index = get_buffer(buff);
    } while(buff_index < 1);

    //decode
    decode_len = huffman_decode(buff,buff_index,decode_buff);

    printf("%s", decode_buff);

  }
}

/*
 * Brief - This function sends out test strings in order to test the functionality of huffman encoding and decoding
 *
 * Note - Do not Modify the test strings. If modified, the same modifications need to made on the KL25Z side as well
 */
void test_huffman()
{

  char * test_case_1 = "$";
  char * test_case_2 = "Hello!";
  char * test_case_3 = "0123456789";
  char * test_case_4 = "12poijadsuf12e90]d'api'af";
  char * test_case_5 = "q[wieofha;kdf;nvajlhaf;dhiulah124y3ro7halbvlajdf;h72rlfah";
  char * test_case_6 = "\nHey! How's it going\n This is a huge string test. Let's see if it works!!!. I'm so excited!!\n";
  char * test_case_7 = "The second string is long as well! This is decoded perfectly as well!!\n";
  char * test_case_8 = "\nDynamic string No. = 1";
  char * test_case_9 = "\nDynamic string No. = 2";
  char * test_case_10 = "\nDynamic string No. = 3";
  char * test_case_11 = "Nov 29 00:24:19 deekshith systemd logind[773]: Lid closed.";
  char * test_case_12 = "Nov 29 14:27:52 deekshith sudo: pam_unix(sudo:session): session opened for user root by (uid=0)";
  char * test_case_13 = "base passwd depends on libdebconfclient0 (>= 0.145); however:";
  char * test_case_14 = "(Reading database ... 55";
  char * test_case_15 = "(Reading database ... 220669 files and directories currently installed.)";
  char * test_case_16 = "dbus daemon[930]: [session uid=999 pid=930] Successfully activated service 'org.gtk.vfs.Metadata";
  char * test_case_17 = "Nov 29 00:24:06 deekshith systemd[16697]: Stopped target GNOME Shell on X11.";
  char * test_case_18 = "Nov 29 00:24:06 deekshith gnome shell[16940]: message repeated 2 times: [ == Stack trace for context 0x559b7d3a4900 ==]";
  char * test_case_19 = "Nov 29 00:24:33 deekshith NetworkManager[758]: <info>  [1638170673.6591] dhcp6 (wlp2s0): state changed bound > done";
  char * test_case_20 = "Nov 29 13:17:11 deekshith /usr/lib/gdm3/gdm x session[36856]: (II) xfree86: Adding drm device (/dev/dri/card1)";
  char * test_case_21 = "Nov 25 17:28:02 deekshith kernel: [65826.859822] OOM killer disabled.";
  char * test_case_22 = "Nov 26 13:32:44 deekshith kernel: [65896.363271] sd 0:0:0:0: [sda] Stopping disk";
  char * test_case_23 = "Nov 26 13:35:17 deekshith kernel: [    0.000000] reserve setup_data: [mem 0x0000000100000000 0x000000027f7fffff] usable";


  printf("\n****************************************\n");
  printf("Starting test cases\nWaiting for data..\n");

  read_and_test(test_case_1);
  read_and_test(test_case_2);
  read_and_test(test_case_3);
  read_and_test(test_case_4);
  read_and_test(test_case_5);
  read_and_test(test_case_6);
  read_and_test(test_case_7);
  read_and_test(test_case_8);
  read_and_test(test_case_9);
  read_and_test(test_case_10);
  read_and_test(test_case_11);
  read_and_test(test_case_12);
  read_and_test(test_case_13);
  read_and_test(test_case_14);
  read_and_test(test_case_15);
  read_and_test(test_case_16);
  read_and_test(test_case_17);
  read_and_test(test_case_18);
  read_and_test(test_case_19);
  read_and_test(test_case_20);
  read_and_test(test_case_21);
  read_and_test(test_case_22);
  read_and_test(test_case_23);
  printf("\n****************Successfully passed all test cases!******************\n");

  g_bits_received = 0;
  g_bits_decoded = 0;
}

/*
* Brief - This funciton is used to read a buffer of data from serial port and compare the decoded string against
*/
void read_and_test(char * test_str)
{
  unsigned char decode_buff[DECODE_BUFF_SIZE];
  uint8_t buff[BUFF_SIZE];
  int buff_index = 0;
  int decode_len = 0;

  //get a buffer of data
  do
  {
    buff_index = get_buffer(buff);
  } while(buff_index < 1);

  //decode
  decode_len = huffman_decode(buff,buff_index,decode_buff);

  printf("\nTest Case Decode Output = %s\n",decode_buff);
  assert(strcmp(test_str,decode_buff)==0);

}
