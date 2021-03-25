# Linux System Programming

This repository contains Linux system programming examples using C language, with a focus on important security concepts and vulnerabilities.

## Table of Contents

- [Linux System Programming](#linux-system-programming)
  - [Table of Contents](#table-of-contents)
  - [Linux OS Core Features](#linux-os-core-features)
  - [Linux System Programming Examples and Documentation](#linux-system-programming-examples-and-documentation)
    - [Command Line Arguments in C](#command-line-arguments-in-c)
  - [Process \& Thread](#process--thread)
    - [The Creation and Extinction of a Process](#the-creation-and-extinction-of-a-process)
    - [System Call exec Group for Program Replacement(Loading) of Processes](#system-call-exec-group-for-program-replacementloading-of-processes)
  - [File System](#file-system)
    - [File I/O functions](#file-io-functions)
    - [Random Access](#random-access)
    - [File Control](#file-control)
  - [Memory Management](#memory-management)
    - [Dynamic Memory Allocation](#dynamic-memory-allocation)
      - [malloc](#malloc)
      - [calloc](#calloc)
      - [free](#free)
      - [realloc](#realloc)
      - [alloca](#alloca)
  - [Memory Mapped File](#memory-mapped-file)
  - [Inter-Process Communication (IPC)](#inter-process-communication-ipc)
    - [IPC using Pipes](#ipc-using-pipes)
    - [Named Pipes and FIFO Files](#named-pipes-and-fifo-files)
  - [Security Essentials](#security-essentials)
    - [Buffer Overflow](#buffer-overflow)
    - [Format String Vulnerabilities](#format-string-vulnerabilities)
    - [Race Conditions](#race-conditions)
    - [Privilege Escalation](#privilege-escalation)

## Linux OS Core Features

- **Multi-user System**
- **Process and Thread-based Multi-tasking**
- **Preemptive and Non-preemptive Scheduling**
- **Multi-CPU Environment Support**
  - SMP: Symmetric Multi Processor
  - NUMA: Non-uniform Memory Access

<br/>

- **High Hardware Portability**
- **Virtual Memory System Support**
  - Demand Paging
  - Page-caching
  - Swap-caching
  - Copy-on-write
  - Pre-loading (OMAGIC)

<br/>

- **Virtual File Systems**
  - Ext2
  - Commercial Unix File Systems
  - VFAT
  - FAT
  - ISO
  - NFS
- **Dynamic Cache**
- **Shared Libraries (ELF)**
- **Networking**
- **GNU C (gcc, g++)**

<br/>

- **POSIX 1003.1-2001, BSD, SysV Compliance**
- **Various Executable File Format Support**
- **International Keyboard and Font Support**
- **Rich Application Ecosystem**
- **Multiple Distributions**
  - RedHat, SuSe, Debian, Slackware, Ubuntu
- **Open Source Software (OSS)**

## Linux System Programming Examples and Documentation

### Command Line Arguments in C

`int main(int argc, char *argv[])`

> `argv` and `argc` are how command line arguments are passed to `main()` in C and C++.
>
> `argc` will be the number of strings pointed to by `argv`. This will (in practice) be 1 plus the number of arguments, as virtually all implementations will prepend the name of the program to the array.
>
> The variables are named `argc` (*argument count*) and `argv` (*argument vector*) by convention, but they can be given any valid identifier: `int main(int num_args, char** arg_strings)` is equally valid.
>
> They can also be omitted entirely, yielding `int main()`, if you do not intend to process command line arguments.

<br/>

## Process & Thread

### The Creation and Extinction of a Process

- Process creation and collaboration by fork : `1_Process_and_Thread/1_print_num/`

### System Call exec Group for Program Replacement(Loading) of Processes

- Example of a small shell using fork and exec system calls : `1_Process_and_Thread/2_shell/`

<br/>

## File System

### File I/O functions

- Example code of file copy : `2_File_System/1_copy/`

### Random Access

- Example code that performs random access and reads/inserts/modifies records in order for a file with 1024 records defined by the record structure : `2_File_System/2_random_access/`

The `lseek` function changes the current read/write offset of a file to a desired position, allowing file access from that position.

```c
#include <sys/types.h>
#include <unistd.h>

off_t lseek(int fildes, off_t offset, int whence);
    
    Input Parameters:
        - filedes : Open file descriptor
        - offset : Position to set
        - whence : Reference point for offset
      SEEK_SET / SEEK_CUR / SEEK_END

    Return Values:
        - Success : New file offset
        - Error : -1
```

### File Control

```c
#include <unistd.h>
#include <fcntl.h>

int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);
int fcntl(int fd, int cmd, struct flock *lock);

// Input Parameters
    - fd : File descriptor to control
    - cmd : Desired file control operation
    - arg : Value required by cmd argument

// Return Values
    - Success : Value depending on cmd
    - Error : -1
```

**Command Options:**

```c
- F_DUPFD : Duplicate file descriptor, returns new file descriptor >= arg value
- F_GETFD : Get close_on_exec flag, returns flag value
- F_SETFD : Set close_on_exec flag according to arg, returns 0
- F_GETFL : Get file status flags, returns current flags
- F_SETFL : Set file status flags (O_APPEND, O_NONBLOCK, O_ASYNC, O_DIRECT), returns 0
```

<br/>

## Memory Management

### Dynamic Memory Allocation

#### malloc

```c
#include <stdlib.h>
void* malloc(size_t size);
```

**Input:**

- size : Size in bytes for memory allocation request

**Return:**

- Success : Pointer to allocated memory block
- Error : NULL

#### calloc

```c
#include <stdlib.h>
void* calloc(size_t nmemb, size_t size);
```

**Input:**

- nmemb : Number of elements to allocate
- size : Size of each element in bytes

**Return:** None

#### free

```c
#include <stdlib.h>
void free(void *ptr)
```

**Input:**

- ptr : Pointer to memory block to be freed

**Return:** None

#### realloc

```c
#include <stdlib.h>
void* realloc(void *ptr, size_t size);  // Reuse existing allocated portion
```

**Input:**

- ptr : Pointer to memory block
- size : Total byte size for reallocation

**Return:**

- Success : Pointer to reallocated memory block
- Error : NULL

#### alloca

```c
#include <alloca.h>
void alloca(size_t size);  // Allocate from stack  // Automatically freed on function return
// Can only be used within the same function
```

**Input:**

- size : Size in bytes to allocate

**Return:**

- Success : Pointer to allocated memory block
- Error : NULL

<br/>

## Memory Mapped File

- Linux operating systems provide a technique that maps the contents of I/O devices or files to a process's virtual address space to increase I/O convenience and access speed. This is called memory mapped I/O, and the related files are called memory mapped files.
- This technique provides generally improved access times by performing direct buffering on mapped user address space pages instead of copying from kernel buffers to user buffers in regular read or write system calls.
- In other words, using this mapping technique, file I/O becomes memory I/O, and processes can access file contents using variables, pointers, arrays, etc., without performing read/write operations on files.

**Example:** `4_Memory_Mapped_File/`

<br/>

## Inter-Process Communication (IPC)

### IPC using Pipes

Unlike regular files that have one read/write offset for one open instance, a pipe has two file descriptors, i.e., two read/write offsets, as if one pipe file was opened twice.

The first descriptor is used for read offset and the other for write offset, making the pipe file work like the front and rear of a queue. These pipes are divided into unnamed pipes used by processes in family relationships through fork inheritance, and named pipes used by processes without such relationships.

```c
#include <unistd.h>

int pipe(int fildes[2]);

// Input Parameters
    - fildes : Two file descriptors for input and output pipes
        
// Return Values
    - Success : 0
    - Error : -1
```

> The input argument above is the two file descriptors returned along with pipe creation.
>
> filedes[0] is used for reading, and filedes[1] is used for writing.
>
> Once a pipe is opened, most APIs such as read and write for regular files can be used.

**Example:** `5_Inter Process Communication/1_file_copy_with_pipe/`

### Named Pipes and FIFO Files

Unnamed pipes are convenient inter-process communication means, but they can only be used to connect processes that can inherit, such as parent and child processes, and have the problem of disappearing when the creating process terminates.

Therefore, named pipes were introduced as a concept that always exists like files, enabling pipe communication between processes without inheritance relationships that can exist in the system at any time.

Named pipes are FIFO (First In First Out) files that exist permanently unless deleted and are given names.

```c
#include <sys/types.h>
#include <sys/stat.h>

int mkfifo (const char *pathname, mode_t mode);

// mkfifo() makes a FIFO special file with name pathname.
// mode specifies the FIFO's permissions.

// Return Values
    - Success : 0
    - Error : -1
```

**Example:** `5_Inter Process Communication/3_named-pipe_and_fifo/`

<br/>

## Security Essentials

This repository includes practical security vulnerability examples to understand common attack vectors and defensive programming techniques.

### Buffer Overflow

Buffer overflow vulnerabilities occur when programs write data beyond the boundaries of allocated memory buffers. This can lead to:

- Stack corruption and program crashes
- Arbitrary code execution
- Return address overwrites
- Privilege escalation

**Key concepts covered:**

- Stack layout and buffer boundaries
- Stack canaries and modern protections
- ASLR (Address Space Layout Randomization)
- NX bit (No Execute) protection
- Compilation flags for vulnerability testing

**Example:** `Security_Essentials/1_Buffer_Overflow/`

The example includes a Makefile for Docker-based testing with:

- Vulnerable and secure compilation options
- Various buffer overflow test scenarios
- Educational exploitation demonstrations
- Security comparison between protected and unprotected binaries

<br/>

### Format String Vulnerabilities

Format string vulnerabilities arise from improper use of printf-family functions where user input is directly used as a format string. This can result in:

- Information disclosure through stack reading
- Arbitrary memory reads
- Arbitrary memory writes
- Program crashes and potential code execution

**Key concepts covered:**

- Format specifier behavior (`%x`, `%s`, `%n`)
- Stack layout analysis
- Position-based format string access
- Memory write techniques
- Compiler warnings and protections

**Example:** `Security_Essentials/2_Format_String/`

The example includes comprehensive testing capabilities:

- Information leakage demonstrations
- Stack content dumping
- Memory write attack simulations
- Crash testing with invalid format strings
- Secure vs. vulnerable implementation comparisons

<br/>

### Race Conditions

Race condition vulnerabilities occur when the security of a program depends on the timing or ordering of events, particularly in multi-threaded or concurrent environments. TOCTOU (Time-of-Check to Time-of-Use) is a common type of race condition.

**Key concepts covered:**

- TOCTOU (Time-of-Check to Time-of-Use) vulnerabilities
- File system race conditions
- Symlink attack vectors
- Atomic operations and secure file handling
- Race window exploitation techniques

**Example:** `Security_Essentials/3_Race_Conditions/`

The example demonstrates:

- Vulnerable file operations with race windows
- Symlink attack simulations
- Secure file handling with proper checks
- Automated race condition testing
- Real-time race condition exploitation

<br/>

### Privilege Escalation

Privilege escalation vulnerabilities allow attackers to gain higher privileges than intended, often through SUID (Set-User-ID) programs or other privileged executables with security flaws.

**Key concepts covered:**

- SUID bit functionality and risks
- Command injection in privileged programs
- Unsafe use of system() function
- Privilege dropping techniques
- Input validation and sanitization

**Example:** `Security_Essentials/4_Privilege_Escalation/`

The example includes:

- SUID binary vulnerability demonstration
- Command injection attack vectors
- Multiple exploitation scenarios
- Secure programming practices
- Privilege management best practices
