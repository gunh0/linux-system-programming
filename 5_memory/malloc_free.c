#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* malloc() */

struct record
{
    int no;
    char name[20];
};

int main()
{
    struct record *ptr;
    ptr = (struct record *)malloc(sizeof(struct record));
    if (ptr)
    {
        ptr->no = 1;
        strcpy(ptr->name, "DevGun");

        printf("Record id is %d.\n", ptr->no);
        printf("Record name is %s.\n", ptr->name);
    }
    free(ptr);
    return 0;
}
