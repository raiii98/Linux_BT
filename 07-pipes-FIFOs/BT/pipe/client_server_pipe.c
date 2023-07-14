#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#define READ_END 0
#define WRITE_END 1
#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)
int main(int argc, char argv[])
{

    int pipe_fd[2];
    pid_t pid;

    if (pipe(pipe_fd) == -1)
    {
        handle_error("pipe()");
    }

    pid = fork();
    if (pid < 0)
    {
        handle_error("fork()");
    }
    else if (pid == 0)
    {
    }
}
