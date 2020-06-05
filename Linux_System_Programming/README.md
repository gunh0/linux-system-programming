## Process & Thread

##### The Creation and Extinction of a Process

- Process creation and collaboration by fork : print_num

##### System Call exec Group for Program Replacement(Loading) of Processes

- Example of a small shell using fork and exec system calls : shell

<br>

## File System

##### File I/O functions

- Example code of file copy : copy

##### Random Access

- Example code that performs random access and reads/inserts/modifies records in order for a file with 1024 records defined by the record structure : random_access

  `lseek` 함수는 파일의 현재 read/write offset을 원하는 위치로 바꾸어 그 위치에서부터 파일에 접근할 수 있도록 한다.

  ```c
  #include <sys/types.h>
  #include <unistd.h>
  
  off_t lseek(int fildes, off_t offset, int whence);
      
      input data
          - filedes : 열린 파일 식별자
          - offset : 설정하고자 하는 위치
          - whence : 초기 위치에 대한 value
      		SEEK_SET / SEEK_CUR / SEEK_END
  
      return data
          - success : offset of new file
          - error : -1
  ```

##### File Control

```c
#include <unistd.h>
#include <fcntl.h>

int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);
int fcntl(int fd, int cmd, struct flock *lock);


# 입력 값
	- fd : 제어하고자 하는 파일 descriptor
	- cmd : 원하는 파일 제어
	- arg : cmd 인수에서 필요한 값

# 반환 값
	- 정상 : cmd에 따른 값
	- 에러 : -1
```

- cmd

  ```c
  - F_DUPFD : dup, arg로 주어진 번호보다 크거나 같은 값에서 dup, 새로운 file descriptor 반환
  - F_GETFD : close_on_exec flag 조사, flag의 값 return
  - F_SETFD : close_on_exec flag를 arg에 따라 set, 0을 return
  - F_GETFL : 파일 상태 flag return
  - F_SETFL : 파일 상태 flag set(O_APPEND, O_NONBLOCK, O_ASYNC, O_DIRECT flag), 0을 return
  ```

<br>

## Memory Management

##### Dynamic Memory Allocation

> ##### malloc
>
> ```c
> #include <stdlib.h>
> void* malloc(size_t size);
> ```
>
> Input
>
> - size : 메모리 할당 요구 바이트 크기
>
> Return
>
> - 정상 : 메모리 블록으로의 포인터
> - 에러 : NULL
>

---

> ##### calloc
>
> ```c
> #include <stdlib.h>
> void* calloc(size_t nmemb, size_t size);
> ```
>
> Input
>
> - nmemb : 할당을 원하는 단위 수
> - size : 메모리 구성요소의 단위 크기
>
> Return : None
>

---

> ##### free
>
> ```c
> #include <stdlib.h>
> void free(void *ptr)
> ```
>
> Input
>
> - ptr : 반환을 원하는 메모리 블록의 포인터
>
> Return : None
>

---

> ##### realloc
>
> ```c
> #include <stdlib.h>
> void* realloc(void *ptr, size_t size);	// 기존 할당 부분 재사용 
> ```
>
> Input
>
> - ptr : 메모리 블록으로의 포인터
> - size : 재할당 하려는 전체 바이트 크기
>
> Return
>
> - 정상 : 할당된 블록으로의 포인터
> - 에러 : NULL
>

---

> ##### alloca
>
> ```c
> #include <alloca.h>
> void alloca(size_t size);	// stack으로 부터 할당	// function return 시 자동 해제
> // 같은 함수 내에서만 사용가능
> ```
>
> Input
>
> - size : 할당을 받으려는 바이트 크기
>
> Return
>
> - 정상 : 할당된 블록으로의 포인터
> - 에러 : NULL

<br/>

## Memory Mapped file

- 리눅스 운영체제에서는 입출력 장치나 파일의 내용을 프로세스의 가상 주소 공간으로 사상(mapping)하여 입출력의 편의성 및 접근 속도를 증가시키는 기법이 제공되는데, 이를 메모리 사상 입출력(memory mapped I/O)이라 하고 이와 관련된 파일을 메모리 사상 파일(memory mapped file)이라 한다.
- 이와 같은 기법은 일반 read 나 write 시스템 호출에서의 커널 버퍼에서 사용자 버퍼로의 복사 대신에, 사상된 사용자 주소 공간의 페이지에 직접적인 버퍼링을 하므로 일반적으로 향상된 접근 시간을 제공한다.
- 즉, 이와 같은 사상 기법을 사용하면 파일로의 입출력이 메모리로의 입출력이 되며, 프로세스는 파일에 대한 읽기/쓰기 작업을 하지 않고 변수, 포인터, 배열 등을 사용하여 파일의 내용에 접근할 수 있다.

<br/>

## Inter-Process Communication (IPC)

##### Pipe 에 의한 IPC

pipe는 일반 파일이 하나의 open instance에 대해 하나의 read/write offset을 갖는 것과는 달리, 하나의 pipe 파일을 마치 두 번 open한 것 처럼 두 개의 file descriptor, 즉, 두 개의 read/write offset을 가진다.

첫 번째 디스크립터는 read offset 용으로 다른 하나는 write offset 용으로 사용되어, 마치 큐의 front와 rear와 같이 사용하는 파일이 pipe이다. 이러한 pipe는 fork에 의한 가족 관계에 있는 프로세스들이 상속하여 사용하는 unnamed pipe와, 이런 관계가 없는 프로세스들이 사용하는 named pipe로 나누어진다.

```c
#include <unistd.h>

int pipe(int fildes[2]);


# 입력 값
	- fildes : 입력 및 출력 파이프를 위한 두 개의 file descriptor
        
# 반환 값
	- 정상 : 0
    - 에러 : -1
```

> 위에서 입력 인수는 pipe 생성과 함께 반환되는 두 개의 file descriptor이다.
>
> filedes[0]는 읽기용으로 사용되고, filedes[1]은 쓰기 용으로 사용된다.
>
> pipe가 오픈되면 일반 파일을 위한 read나 write 등 대부분의 API를 사용할 수 있다.

<br/>

##### Named pipe와 FIFO 파일

unnamed 파이프는 편리한 프로세스 간 통신 수단이지만 부모와 자식 프로세스와 같이 상속이 가능한 프로세스들을 연결하는 데만 사용할 수 있으며 생성 프로세스가 종료되면 함께 사라지는 문제점이 있다.

따라서 서로 상속 관계가 없는 프로세스들끼리 언제라도 시스템에 존재하는 pipe 통신을 하기 위해서 파일과 같이 항상 존재하는 개념인 named pipe가 도입되었다.

named pipe는 지우지 않는한 영구히 존재하며 이름도 부여받는 FIFO(First In First Out) 파일이다.

````c
#include <sys/types.h>
#include <sys/stat.h>

int mkfifo (const chat *pathname, mode_t mode);


// mkfifo() makes a FIFO special file with name pathname.
// mode specifies the FIFO's permissions.

# 반환 값
	- 정상 : 0
    - 에러 : -1
````