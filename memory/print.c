#include <unistd.h>
#include <stdio.h>

extern etext, edata, end;
int main(void)
{
    char *break_point;
    printf("exext value : %x\n", &etext);
    printf("edata value : %x\n", &edata);
    printf("end value : %x\n", &end);
    break_point = sbrk(0);
    printf("current break value -> %x\n", break_point);
}

/*
int *sbrk(ptrdiff_t increment);

입력 값
    - increment : 현재의 break값에 더해지는 값

반환 값
    - 정상 : 이전의 break 값
    - 에러 : -1
 */