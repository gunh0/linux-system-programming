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

