/*
* Brief - Consists of necessary funcitons to open, read and write to a serial port
* Author - Deekshith Reddy Patil, patil.deekshithreddy@colorado.edu
*/

#include "serial.h"

struct termios g_tty;
int g_fd;

static int file_open_and_get_descriptor(const char *fname);
static int file_write_data(int fd, uint8_t *buff, uint32_t len_buff);
static int file_close(int fd);

//Refer "serial.h" for function description
void open_serial_port(char * serial_port)
{
    g_fd = file_open_and_get_descriptor(serial_port);
    if(g_fd < 0)
    {
        printf("Something went wrong while opening the port...\r\n");
        exit(EXIT_FAILURE);
    }
}

//Refer "serial.h" for function description
void configure_serial_port(void)
{
    if(tcgetattr(g_fd, &g_tty))
    {
        printf("Something went wrong while getting port attributes...\r\n");
        exit(EXIT_FAILURE);
    }

    cfsetispeed(&g_tty,BAUD_RATE);
    cfsetospeed(&g_tty,BAUD_RATE);

    cfmakeraw(&g_tty);

    if(tcsetattr(g_fd,TCSANOW,&g_tty))
    {
        printf("Something went wrong while setting port attributes...\r\n");
        exit(EXIT_FAILURE);
    }
}

//Refer "serial.h" for function description
void close_serial_port(void)
{
    file_close(g_fd);
}

//Refer "serial.h" for function description
int file_read_data(int fd, uint8_t *buff, uint32_t len_buff)
{
    return read(fd,buff,len_buff);
}

/*
* Brief - This function is used to open a file , "fname"(and inturn a serial port).
*       - In linux, Serial ports are implemented as files
*/
static int file_open_and_get_descriptor(const char *fname)
{
    int fd;

    fd = open(fname, O_RDWR | O_NONBLOCK);
    if(fd < 0)
    {
        printf("Could not open file %s...%d\r\n",fname,fd);
    }
    return fd;
}

/*
* Brief - This function is used to write data into the file 'fd' (and inturn into a serial port)
*       - 'buff' holds the message that needs to be written and 'len_buff' holds the length of buff
*/
static int file_write_data(int fd, uint8_t *buff, uint32_t len_buff)
{
    return write(fd,buff,len_buff);
}

/*
* Brief - Helper function to close a file
*/
static int file_close(int fd)
{
    return close(fd);
}
