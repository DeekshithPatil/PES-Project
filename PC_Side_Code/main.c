#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#define ERROR_CODE_UNABLE_TO_OPEN_SECIFIED_PORT         -1

int main(int argc, char * argv[])
{
    char byte;

    printf("COM PORT = %s\n", argv[1]);
    int port = open(argv[1], O_RDWR);

    assert(port != ERROR_CODE_UNABLE_TO_OPEN_SECIFIED_PORT);
    printf("port= %d\n", port);

    while(1)
    {
      read(port, &byte, 1);
      printf("%c", byte);
    }


    return 0;
}
