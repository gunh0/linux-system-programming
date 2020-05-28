/* 메모리 사상 입출력에 의한 병행 프로세스의 파일

아래 코드에서 타겟 파일이 일단 생성되었어도 실질적으로 그 내용은 존재하지 않으므로 크기는 0이고
이와 같은 파일에 대해 그보다 큰 크기의 주소 공간을 사상하는 것은 불가능하다.
따라서 타겟 파일에 사상하려면 파일의 마지막 부분에 write를 실행하여 소스 파일과 같은 크기로 만든 후 사상하여야 한다.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define OPEN_MODE S_IRUSR | S_IWUSR

void error_exit(char *message)
{
    printf("stderr, %s\n", message);
    exit(-1);
}

int main()
{
    int fdin, fdout;
    char *src;
    char *dst;
    struct stat statbuf;
    int filesize;

    fdin = open("input", O_RDONLY);
    fdout = open("output", O_RDWR | O_CREAT | O_TRUNC, OPEN_MODE);

    printf("fdout = %d\n", fdout);
    printf("open succ\n");

    fstat(fdin, &statbuf);
    filesize = statbuf.st_size;
    printf("source file size = %d\n", filesize);
    lseek(fdout, filesize - 1, SEEK_SET);
    write(fdout, "", 1);

    if ((src = (char *)mmap((void *)0, filesize, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED)
        error_exit("mmap 1 failed");

    if ((dst = (char *)mmap((void *)0, filesize, PROT_WRITE | PROT_READ, MAP_SHARED, fdout, 0)) == MAP_FAILED)
        error_exit("mmap 2 failed");

    printf("mmap success\n");
    fflush(stdout);

    if (fork() == 0)
    {
        memcpy(dst, src, filesize / 2);
        munmap(src, filesize);
        munmap(dst, filesize);
        exit(0);
    }

    int status;
    memcpy(dst + filesize / 2, src + filesize / 2, filesize - filesize / 2);
    wait(&status);
    printf("copied file = %20s\n", dst);
    munmap(src, filesize);
    munmap(dst, filesize);
}