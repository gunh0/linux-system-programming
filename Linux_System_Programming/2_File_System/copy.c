#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#ifndef BUF_SIZE
#define BUF_SIZE 4096 //4KB
#endif

int main(int argc, char *argv[])
/* cp orig dest */
{
	if (argc != 3)
	{
		printf("USAGE : %s <origin file> <destination file>\n", argv[0]);
		return 0;
	}
	int fd_orig, fd_dest, n;
	char *buf;
	if (fd_orig = open(argv[1], O_RDONLY))
	{
		buf = malloc(4096);
		if (fd_dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC), 0777)
		{
			do
			{
				n = read(fd_orig, buf, BUF_SIZE);
				write(fd_dest, buf, n);
				/* write(int 파일 서술자,
                const void* 파일 서술자에 쓰려고 하는 데이터,
                size_t 쓰려는 바이트의 수); */
			} while (n == BUF_SIZE);
		}
		else
		{
			printf("Failed to open %s\n", argv[2]);
			close(fd_orig);
			return -1;
		}
		printf("%s", buf);
		while (!close(fd_orig))
		{
			exit(EXIT_FAILURE);
		};
		while (!close(fd_orig))
		{
			exit(EXIT_FAILURE);
		};
		free(buf);
	}
	else
	{
		exit(EXIT_FAILURE);
	}

	return 0;
}