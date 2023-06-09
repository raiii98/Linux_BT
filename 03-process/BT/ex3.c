#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int status, value;

// Đề bài
// Khi một child process kết thúc,
// nó sẽ gửi signal nào tới parent?
// signal value bằng bao nhiêu?

void notify()
{
    wait(&status);
    if (WIFSIGNALED(status))
    // Trường hợp tiến trình con kết thúc 1 cách bất thường, in ra signal đó
    {
        printf("Child process terminated by signal %d\n", WTERMSIG(status));
    }
    else
    // Trường hợp tiến trình con kết thúc 1 cách bình thường
    // Nếu tiến trình con vẫn tiếp tục chạy cho đến khi nhận 1 signal
    // từ hệ thống, in ra signal đó.
    {
        printf("Child process terminated by signal %d\n", SIGCHLD);
    }
}
int main(int argc, char *argv[])
{
    pid_t pid;

    pid = fork(); // khởi tạo tiến trình con bằng hàm fork.
    if (pid == 0)
    {
        printf("I'm the child process 1\n");
        /* tiến trình thực hiện lỗi và gửi 1 signal về tiến trình cha. */
        int ans = 1 / 0; // signal in ra là 8 nếu tiến trình lỗi tại đây
        while (1)
        {
            /* code */
        }
        exit(0);
    }
    else
    {
        // SIGCHLD gửi tín hiệu đến tiến cha khi tiến trình con kết thúc
        // hoặc thay đổi trạng thái
        signal(SIGCHLD, notify);
        // signal in ra là 17 nếu tiến trình con kết thúc bình thường
        /* signal in ra là 1-31 nếu tiến trình con thay đổi trạng thái
        theo  Signal numbering for standard signals */

        printf("I'm the parent process\n");

        while (1)
        {
            /* code */
        }
    }
}
