#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int global_int = 5;
int main()
{
    pid_t pid;

    if ((pid = fork()) == 0) // child
    {
        global_int = 6;
        printf("child's data = %d\n", global_int);
        exit(0);
    }
    else // parent
    {
        global_int = 7;
        printf("parent's data = %d\n", global_int);
        wait(NULL);
    }

    // parent : after join

    printf("parent's data = %d", global_int);
    return 0;
}