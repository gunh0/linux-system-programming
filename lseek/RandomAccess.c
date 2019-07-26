/*
off_t lseek(int fildes, off_t offset, int whence);
    
    input data
        - filedes : 열린 파일 식별자
        - offset : 설정하고자 하는 위치
        - whence : 초기 위치에 대한 value
            SEEK_SET / SEEK_CUR / SEEK_END

    return data
        - success : offset of new file
        - error : -1
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUM_RECORDS 1024

struct record{
    int id;
    char name[20];
};

void input_record_contents(struct record *current)
{
    printf("record id = ");
    scanf("%d%*c", &(current->id));

    printf("name = ");
    scanf("%s%*c", &(current->name));
}

int main()
{
    struct record current;
    int record_no;
    int fd, pos, i, n;
    char yes;
    fd = open("testdata", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    current.id=-1;  // empty record
    //initialize with empty record
    for(i=0;i<NUM_RECORDS;++i)
        write(fd, &current, sizeof(struct record));
    
    printf("enter record number\n");
    scanf("%d%*c", &record_no);

    while(record_no>=0&&record_no<NUM_RECORDS)
    {
        pos=record_no*sizeof(struct record);
        lseek(fd, pos, SEEK_SET);

        n = read(fd, &current, sizeof(struct record));

        if(current.id==-1)
            printf("record empty\n");
        else{
            printf("record id = %d\n",current.id);
            printf("name=%s \n",current.name);
        }

        printf("update or insert? yes = y\n");
        scanf("%c%*c", &yes);

        if(yes == 'y')
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