#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>    

int main() {
    char byte;
    int fd = open("/dev/ttyACM0", O_RDWR);
    ssize_t size = 0;
    
    char str[100] = "";
    int i = 0;
    while(1)
    {

	do
	{
		size = read(fd, &byte, 1);
	}while(size == 0);
	
	if(byte == '\0')
	{
		str[i] = '\0';
		printf("%s\n",str);
		i = 0;
	}
	
	else
	{
		str[i] = byte;
		i = (i + 1)%100;
	}
    }

    return 0;
}
