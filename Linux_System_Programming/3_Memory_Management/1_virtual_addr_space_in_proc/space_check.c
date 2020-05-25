#include <unistd.h>
#include <stdio.h>

extern int etext, edata, end;

int main()
{
    char *break_point;
    printf("exext value : %p\n", &etext);
    printf("edata value : %p\n", &edata);
    printf("end value : %p\n", &end);
    break_point = sbrk(0);
    printf("current break value -> %p\n", break_point);

    return 0;
}