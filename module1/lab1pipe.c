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

    // Child
    if (pid == 0)
    {
        close(fd[0]);

        // bind STDOUT to fd[1], so that output from any process gets sent through the pipe
        dup2(fd[1], STDOUT_FILENO);
        execlp("ls", "ls", "/", NULL);

        close(fd[1]);
    }
    // Parent
    else if (pid > 0)
    {
        close(fd[1]);

        // wait for child to finish execution
        wait(NULL);

        // bind STDIN to fd[0], so that processes reading from STDIN will read from fd[0] instead
        dup2(fd[0], STDIN_FILENO);
        execlp("wc", "wc", "-l", NULL);

        close(fd[0]);
    }
}