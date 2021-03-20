/* Memory mapped I/O for parallel process file operations

In the code below, even though the target file is created, its content doesn't actually exist, so its size is 0.
It's impossible to map an address space larger than the file size to such a file.
Therefore, to map to the target file, you must execute a write at the end of the file to make it the same size as the source file before mapping.
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