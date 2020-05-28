#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct record{
    int no;
    char name[20];
};

int main(void)
{
    struct record *ptr;
    ptr = (struct record*)malloc(sizeof(struct record));
    if(ptr)
    {
        ptr->no=1;
        strcpy(ptr->name, "DevGun");

        printf("record id %d\n",ptr->no);
        printf("record name %s\n",ptr->name);
    }
    free(ptr);
    return 0;
}