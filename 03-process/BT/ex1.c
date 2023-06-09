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
            exit(0); // kết thúc bình thường gửi 1 thông báo với giá trị trong ham exit
        }
        else
        {

            result = waitpid(-1, &status, 0);
            printf("I am the parent process\n");
            printf("My PID is %d, ", getpid());
            if (WIFEXITED(status))
            {
                printf("I exited with status %d\n", WEXITSTATUS(status));
                // WEXITSTATUS lấy ra giá trị được thông báo bởi chương trình con
                // Khi mà chương trình con kết thúc 1 cách bình thường
            }
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