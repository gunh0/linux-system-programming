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