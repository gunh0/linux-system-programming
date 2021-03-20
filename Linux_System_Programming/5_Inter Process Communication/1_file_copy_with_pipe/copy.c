/* Example of file copy using pipe between parent and child processes

The parent process copies file contents received from the child through a pipe to a new file.
Since both processes have different speeds, the parent will wait when the pipe becomes empty.
When file transmission is complete, the child closes the write channel of the pipe,
so the parent no longer waits and the number of bytes read returns 0, terminating the program.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

void error_exit(char *message);

int main()
{
    int fd[2];         // two descipters for pipe
    int input, output; // copy input and output
    int n;
    char buf[10];

    if (pipe(fd) == -1)
        exit(1);

    // child process
    if (fork() == 0)
    {
        input = open("input", O_RDONLY); // open for reading
        if (input == -1)
            exit(1);

        close(fd[0]); // close read pipe channel

        while ((n = read(input, buf, 10)) != 0) // until EOF
            write(fd[1], buf, n);

        close(input);
        close(fd[1]);

        exit(0);
    }

    else
    {
        close(fd[1]); // close unnecessary writing channel
        output = creat("output", 0666);
        if (output == -1)
            error_exit("creat");

        while ((n = read(fd[0], buf, 10)) != 0) // until pipe closed
            write(output, buf, n);

        close(output);
        close(fd[0]);
        wait(0);
    }

    return 0;
}

void error_exit(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(-1);
}