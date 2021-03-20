#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGE "message to stdout \n"

int main(void)
{
    int newfd;
    
    if((newfd=fcntl(STDERR_FILENO,F_DUPFD,3))==-1){
        perror("fcntl");
        exit(1);
    }

    close(STDOUT_FILENO);
    write(3,MESSAGE,strlen(MESSAGE));   // Actually outputs to monitor
    return 0;
}