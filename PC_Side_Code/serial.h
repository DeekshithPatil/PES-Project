/*
* Brief - Consists of necessary funcitons to open, read and write to a serial port
* Author - Deekshith Reddy Patil, patil.deekshithreddy@colorado.edu
* Implementation credits: https://www.youtube.com/watch?v=tUJ-wTafQLA, https://github.com/sckulkarni246/ke-rpi-samples/blob/main/uart-c-termios/uart_loopback.c
*/

#ifndef __SERIAL_H__
#define __SERIAL_H__

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

#define BAUD_RATE                                    B115200
#define TOTAL_ACCEPTABLE_TERMINAL_COMMANDS           2
extern int g_fd;

/*
* Brief - This function is used to open a "serial_port" for commmunication
*/
void open_serial_port(char * serial_port);

/*
* Brief - This function is used to configure a serial port with appropriate parameters
*/
void configure_serial_port(void);

/*
* Brief - Used to close the already open "serial_port"
*/
void close_serial_port(void);

/*
* Brief - This function is used to read data from a file 'fd' (and inturn from a serial port)
*       - 'buff' holds the message that is read and 'len_buff' holds the length of buff
*/
int file_read_data(int fd, uint8_t *buff, uint32_t len_buff);

#endif
