#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

int receiver(int *);
int sender(int *);

int main(void)
{
    int p[2];
    if (pipe(p) == -1)
        exit(1);

    // p[0] : Set Non-blocking Mode
    if (fcntl(p[0], F_SETFL, O_NONBLOCK) == -1)
        exit(1);

    if (fork() == 0) // child
        sender(p);
    else // parent
        receiver(p);

    return 0;
}

int receiver(int *p)
{
    int n, buf;
    close(p[1]);
    while ((n = read(p[0], &buf, sizeof(int))) != 0)
    {
        // until pipe closed
        switch (n)
        {
        case -1: // Inspect pipe for nothing
            if (errno == EAGAIN)
            {
                printf("empty pipe\n");
                sleep(1);
                break;
            }
            else
            {
                exit(1);
            }
        default: // data received
            printf("data=%d\n", buf);
        }
    }
    printf("end of processing\n");
    exit(0);
}

int sender(int *p)
{
    int count;
    close(p[0]);
    for (count = 0; count < 10; count++)
    {
        write(p[1], &count, sizeof(int));
        sleep(2);
    }

    // pipe close
    close(p[1]);
    exit(0);
}