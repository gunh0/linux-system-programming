#include <stdio.h>
#include <sys/wait.h> // Wait
#include <stdlib.h> // Exit
#include <fcntl.h> // File : O_RDONLY
#include <unistd.h> // Read, Pipe

int main() {
	int n, input, output; // Input File & Output File
	int fd1[2]; // First File Descriptor
	int fd2[2]; // Second File Descriptor
	char buff[10];
	int status;
	pid_t pid;

	if (pipe(fd1) == -1 || pipe(fd2) == -1)
		exit(1);

	if ((pid = fork()) == 0) { // Child 1
		close(fd1[1]); // Close unnecessary writing channel
		while ((n = read(fd1[0], buff, 10)) != 0)
		{
			// Convert into capital
			for (int i = 0; i < 10; i++)
			{
				if (buff[i] >= 'a' && buff[i] <= 'z')
					buff[i] = buff[i] - 32;
			}
			close(fd2[0]); // Close read pipe channel
			// Put characters into pipe 2
			write(fd2[1], buff, n);
		}
		close(fd1[0]);
		close(fd2[1]);
		exit(0);
	}

	if ((pid = fork()) == 0) { // Child 2
		close(fd2[1]);
		output = creat("output.txt", 0666);
		while ((n = read(fd2[0], buff, 10)) != 0) // Get 10 bytes from pipe 2 until pipe 2 colsed
			write(output, buff, n); // Write characters intp output file
		close(output);
		close(fd2[0]);
		exit(0);
	}

	else { // Parent
		input = open("text.txt", O_RDONLY); // File Open
		if (input > 0) // File Open Success
		{
			close(fd1[0]);
			while ((n = read(input, buff, 10)) != 0)
				write(fd1[1], buff, n); //Put them into pipe 1
			close(input);
			close(fd1[1]);
			wait(&status);
			wait(&status);
		}
		else // File Open Error
		{
			printf("File Error ! \n");
			return 0;
		}
	}
	return 0;
}