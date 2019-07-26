#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Special Function on C
* printf : 표준출력에 출력, 인자값을 여러개 쓸 수 있다.
*	1) format string : 문자상수, Place Holder(치환자, %d, %x, %s, %c ...)
* 	2) 상수, 변수, 포인터 등등... Place Holder의 갯수와 동일하게 주어져야 하지만, 그렇지 않아도 컴파일은 가능하다.
* scanf : 표준입력의 값을 출력 / 인자값에 지정한 포인터 변수에 메모를 복사함.
*	scanf("format string %s %d %x %h %c ..", &p, ... );
*	(in Windows) scanf_s : 입력받은 값을 확인해줌
* main : 프로그램의 시작점. 본체.
*/

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