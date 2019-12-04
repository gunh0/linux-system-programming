#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    int flag;

    // read the close-on-exec flag
    if((flag=fcntl(STDIN_FILENO, F_GETFD))==-1){
        perror("fcntl");
        exit(1);
    }

    printf("close on exec flag = %d\n",flag);
    return 0;
}