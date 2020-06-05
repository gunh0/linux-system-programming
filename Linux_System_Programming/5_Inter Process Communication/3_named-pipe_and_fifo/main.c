// message sender
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 80

int main(int argc, char **argv)
{
    char *fifo_file = "my_fifo.txt";
    char buf[SIZE + 1];
    int fd;
    int i, n;

    if (argc < 2)
        exit(1);

    // non-blocking mode
    if ((fd = open(fifo_file, O_WRONLY | O_NONBLOCK)) < 0) // wirte only + non-blocking
        exit(0);

    // send message
    for (i = 1; i < argc; ++i)
    {
        if (strlen(argv[i]) > SIZE)
        {
            printf("mssg too long\n");
            printf("buf size : %d\n", SIZE);
            continue;
        }
        strcpy(buf, argv[i]);
        printf("%s\n", buf);
        //if ((n = write(fd, buf, SIZE + 1)) == -1)
        if ((n = write(fd, buf, strlen(buf))) == -1)
            exit(1);
    }
    exit(0);

    return 0;
}