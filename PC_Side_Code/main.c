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

#define NO_BYTE_AVAILABLE                 -1

/*
* Brief - Used to start reading data from the serial port
*/
void start_serial_read()
{
    uint8_t data = 0;
    int len = 0;
    printf("Starting serial read!\nWaiting for data..\n");
    while(1)
    {
      do
      {
        len = file_read_data(g_fd,&data,1);
      } while(len == NO_BYTE_AVAILABLE);

      if(data!=0)
        printf("%c %x\n",data,data);
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
