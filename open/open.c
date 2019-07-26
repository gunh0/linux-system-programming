#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;

	if (argc > 1)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			printf("Invalid file name : %s\n", argv[1]);
		}
		else
		{
			printf("Success to open file : %s\n", argv[1]);
			close(fd);
		}
	}

	else
	{
		printf("Usage : %s <filename>\n", argv[0]);
	}

	return 0;
}