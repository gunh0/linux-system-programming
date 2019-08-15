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
