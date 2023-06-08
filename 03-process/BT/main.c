#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int status, result;
    pid_t child_pid = fork();
    if (child_pid >= 0)
    {
        if (child_pid == 0)
        {
            printf("I am the child process\n");
            printf("My PID is %d, My parent is %d\n", getpid(), getppid());
            exit(0);
        }
        else
        {

            result = waitpid(-1, &status, 0);
            printf("I am the parent process\n");
            printf("My PID is %d, ", getpid());
            printf("Child status is %d\n", WEXITSTATUS(status));
        }
    }
    else
    {
        printf("fork failed\n");
    }
    while (1)
    {
        /* code */
    }
}