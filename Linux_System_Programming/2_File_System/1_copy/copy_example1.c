// 시스템 함수들을 이용하여 한 파일을 다른 파일로 복사하는 프로그램
// outfile.out 'source file: input.txt' 'copy file: output.txt'

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> /* exit() */
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int fdin, fdout, n;
    char buf[BUFFER_SIZE];

    if (argc != 3)
    {
        perror(argv[0]);
        exit(1);
    }

    if ((fdin = open(argv[1], O_RDONLY)) == -1)
    {
        perror(argv[1]);
        exit(1);
    }

    //if ((fdout = open(argv[2], O_RDONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
    if ((fdout = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC), 0644) == -1)
    {
        perror(argv[2]);
        exit(1);
    }

    while ((n = read(fdin, buf, BUFFER_SIZE)) != 0)
        write(fdout, buf, n);

    close(fdin);
    close(fdout);
}
