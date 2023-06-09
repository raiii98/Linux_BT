#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void notify()
{
    printf("The signal child process %d\n", SIGCHLD);
    wait(NULL);
}
int main(int argc, char *argv[])
{

    pid_t child_pid = fork();

    {
        if (child_pid == 0)
        {
            printf("I am the child process\n");
            // while (1)
            //     ;
        }
        else
        {
            signal(SIGCHLD, notify);
            printf("I am the parent process\n");
            while (1)
                ;
        }
    }
}
