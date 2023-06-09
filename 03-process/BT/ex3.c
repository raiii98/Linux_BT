#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int status, value;

    pid = fork();
    if (pid == 0)
    {
        printf("I'm the child process 1\n");
        int ans = 1 / 0; // chương trình thực hiện lỗi và gửi về 1 signal về
                         // chương trình cha.
        exit(0);
    }
    else
    {

        wait(&status);
        if (WIFSIGNALED(status)) // WIFSIGNALED trả về true nếu chương trình
                                 // con kết bằng sighnal
        {

            printf("Child process terminated by signal %d\n", WTERMSIG(status));
            // WTERMSIG lấy ra signal mà chương trình con đã gửi.
        }
        printf("I'm the parent process\n");
        while (1)
        {
            /* code */
        }
    }
}
