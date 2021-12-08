#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

int main(int argc, char * argv[])
{
    char byte;
    ssize_t size;

    printf("COM PORT = %s\n", argv[1]);
    int port = open(argv[1], O_RDWR);

    assert(port != -1);
    printf("port = %d\n", port);

    while(1)
    {
      size = read(port, &byte, 1);
      printf("%c", byte);
    }


    return 0;
}
