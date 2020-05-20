/*
레코드의 번호를 입력받아 lseek와 read 후, 이 레코드에 다시 수정된 내용을 입력할 때에는 다시 lseek를 하여야 함에 주의 하여야 한다.
이는 read에 의해 offset이 레코드의 크기만큼 앞으로 나기 때문이다.
또한, 입력하는 레코드 번호가 마지막에 저장된 레코드의 번호보다 클 때에 lseek와 read를 수행하면 EOF로 반환되므로 통상 최대 레코드의 수를 정하고 마지막에는 마지막 표시 레코드를 미리 삽입해두어야 한다.
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
