#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
    int fd[2];
    pipe(fd);

    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        // Child
        dup2(fd[1], STDOUT_FILENO);
        execlp("ls", "ls", "/", NULL);
        close(fd[1]);
    }
    else if (pid > 0)
    {
        // Parent

        // wait for child to finish execution
        wait(NULL);

        // close unused pipe ends and read from child
        close(fd[1]);

        dup2(fd[0], STDIN_FILENO);
        execlp("wc", "wc", "-l", NULL);

        close(fd[0]);
    }
}