/*
When inputting a record number and performing lseek and read, note that you must perform lseek again when inputting modified content back to this record.
This is because the offset moves forward by the size of the record due to the read operation.
Also, when the input record number is greater than the last stored record number, performing lseek and read will return EOF, so it's common to set a maximum number of records and pre-insert an end marker record at the end.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NUM_RECORDS 1024

struct record
{
    int id;
    char name[20];
};

void input_record_contents(struct record *current)
{
    printf("record id = ");
    scanf("%d%*c", &(current->id));

    printf("name = ");
    scanf("%s%*c", current->name);
}

int main()
{
    struct record current;
    int record_no;
    int fd, pos, i, n;
    char yes;
    fd = open("running_data", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    current.id = -1; // empty record
    //initialize with empty record
    for (i = 0; i < NUM_RECORDS; ++i)
        write(fd, &current, sizeof(struct record));

    printf("enter record number\n");
    scanf("%d%*c", &record_no);

    while (record_no >= 0 && record_no < NUM_RECORDS)
    {
        pos = record_no * sizeof(struct record);
        lseek(fd, pos, SEEK_SET);

        n = read(fd, &current, sizeof(struct record));

        if (current.id == -1)
            printf("record empty\n");
        else
        {
            printf("Record id = %d\n", current.id);
            printf("Recode name = %s \n", current.name);
        }

        printf("update or insert? yes = y\n");
        scanf("%c%*c", &yes);

        if (yes == 'y')
        {
            printf("enter new contentes\n");
            input_record_contents(&current);
            lseek(fd, pos, SEEK_SET);
            write(fd, &current, sizeof(struct record));
        }

        printf("enter next record number, -1 = EXIT\n");
        scanf("%d%*c", &record_no);
    }
    close(fd);
    return 0;
}
