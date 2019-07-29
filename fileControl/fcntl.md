```c
#include <unistd.h>
#include <fcntl.h>

int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);
int fcntl(int fd, int cmd, struct flock *lock);
```

### 입력 값

- fd : 제어하고자 하는 파일 descriptor
- cmd : 원하는 파일 제어
- arg : cmd 인수에서 필요한 값

### 반환 값

- 정상 : cmd에 따른 값
- 에러 : -1

---

		- F_DUPFD : dup, arg로 주어진 번호보다 크거나 같은 값에서 dup, 새로운 file descriptor 반환
		- F_GETFD : close_on_exec flag 조사, flag의 값 return
		- F_SETFD : close_on_exec flag를 arg에 따라 set, 0을 return
		- F_GETFL : 파일 상태 flag return
		- F_SETFL : 파일 상태 flag set(O_APPEND, O_NONBLOCK, O_ASYNC, O_DIRECT flag), 0을 return