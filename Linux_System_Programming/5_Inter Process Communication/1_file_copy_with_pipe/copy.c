/* parent 와 child 프로세스의 pipe 에 의한 파일 복사의 예

parent 프로세스는 child 로부터 pipe 를 통해 전달되는 파일의 내용을 새로운 파일에 목사하는데,
양 프로세스의 속도가 다르므로 pipe 가 비게될 경우에 parent 는 대기하게 된다.
파일의 전송이 모두 완료되면 child 가 pipe 의 write 채널을 close 하게 되어
parente 는 더는 대기하지 않고 읽은 byte의 수가 0으로 return 되어 프로그램을 종료하게 된다.
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