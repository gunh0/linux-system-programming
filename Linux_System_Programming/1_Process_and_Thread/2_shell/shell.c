#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h> /* exit() */
#include <string.h> /* strlen() */
#include <stdio.h>

#define PROMPT "$>"
#define LINE_LENGTH 80

int main(void)
{
    char buffer[LINE_LENGTH];
    pid_t pid;
    int c, count = 0;
    int background = 0;

    for (;;)
    {
        printf(PROMPT);
        count = 0;
        while (1)
        {
            if ((c = getchar()) == EOF)
                exit(0); // normal exit

            if (count < LINE_LENGTH)
                buffer[count++] = c;

            if (c == '\n' && count < LINE_LENGTH)
            {
                buffer[count - 1] = 0;
                break;
            }

            if (count >= LINE_LENGTH)
            {
                printf("input too long\n");
                count = 0;
                printf(PROMPT);
                continue;
            }
        } // end of while

        if (strlen(buffer) == 0 || (strlen(buffer) == 1 &&
                                    buffer[strlen(buffer) - 1] == '&'))
            continue; // to for loop

        if (buffer[strlen(buffer) - 1] == '&')
        {
            background = 1;
            buffer[strlen(buffer) - 1] = '\0';
        }

        if ((pid = fork()) < 0)
        {
            perror("fork failed");
            exit(1);
        }
        else if (pid == 0) // child process
        {
            execlp(buffer, buffer, (char *)0);
            perror("exclp failed");
            exit(1);
        }

        if (background = 1)
            continue;

        while (wait(&pid) != pid)
            ; // wait for the exit of foreground

    } // end of for
}
