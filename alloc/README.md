### malloc

```c
#include <stdlib.h>
void* malloc(size_t size);
```

Input

- size : 메모리 할당 요구 바이트 크기

Return

- 정상 : 메모리 블록으로의 포인터
- 에러 : NULL

---

### calloc

```c
#include <stdlib.h>
void* calloc(size_t nmemb, size_t size);
```

Input

- nmemb : 할당을 원하는 단위 수
- size : 메모리 구성요소의 단위 크기

Return : None

---

### free

```c
#include <stdlib.h>
void free(void *ptr)
```

Input

- ptr : 반환을 원하는 메모리 블록의 포인터

Return : None

---

### realloc

```c
#include <stdlib.h>
void* realloc(void *ptr, size_t size);	// 기존 할당 부분 재사용 
```

Input

- ptr : 메모리 블록으로의 포인터
- size : 재할당 하려는 전체 바이트 크기

Return

- 정상 : 할당된 블록으로의 포인터
- 에러 : NULL

---

### alloca

```c
#include <alloca.h>
void alloca(size_t size);	// stack으로 부터 할당	// function return 시 자동 해제
// 같은 함수 내에서만 사용가능
```

Input

- size : 할당을 받으려는 바이트 크기

Return

- 정상 : 할당된 블록으로의 포인터
- 에러 : NULL