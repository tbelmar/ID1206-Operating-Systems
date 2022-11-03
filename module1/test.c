#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
    pid_t pid;

    int fd[2];

    pipe(fd);
    pid = fork();

    if (pid == 0)
    {
        // Child
        close(fd[0]);

        int x = dup(STDOUT_FILENO);
        // execl("/usr/bin/ls", "ls", "/", NULL);
        dup2(fd[1], STDOUT_FILENO);
        puts("test");

        dup2(STDOUT_FILENO, x);
        puts("test2");
        // execl("/usr/bin/ls", "ls", "/", NULL);

        exit(0);
    }
    else if (pid > 0)
    {
        // Parent

        char child_msg[1000];

        // wait for child to finish execution
        wait(NULL);

        // close unused pipe ends and read from child
        close(fd[1]);
        read(fd[0], child_msg, 1000);
        close(fd[0]);

        execl("/usr/bin/wc", "wc", "-l", child_msg, NULL);
    }
    else
    {
        // Error
        printf("Error when forking.\n");
    }
}