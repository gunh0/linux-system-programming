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
{
	if (argc != 3)
	{
		printf("USAGE : %s <origin file> <destination file>\n", argv[0]);
		return 0;
	}
	int fd_orig, fd_dest;
	char *buf;
	if (fd_orig = open(argv[1], O_RDONLY))
	{
		int cnt;
		buf = malloc(4096);
		if (fd_dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC), 0644)
		{
			do
			{
				cnt = read(fd_orig, buf, BUF_SIZE);
				write(fd_dest, buf, cnt);
				/* write(int 파일 서술자,
                const void* 파일 서술자에 쓰려고 하는 데이터,
                size_t 쓰려는 바이트의 수); */
			} while (cnt == BUF_SIZE);
		}
		else
		{
			printf("Failed to open %s\n", argv[2]);
			close(fd_orig);
			return -1;
		}
		printf("%s", buf);
		close(fd_dest);
		close(fd_orig);
	}
	free(buf);
	return 0;
}