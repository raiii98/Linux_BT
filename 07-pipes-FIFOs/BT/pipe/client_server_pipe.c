#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 1048
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

        dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
        printf("aaaa");
        close(pipe_fd[READ_END]);

        execlp("ack", "ack", "Chil101", NULL);
        exit(EXIT_SUCCESS);
    }
    else
    {

        char output[BUFFER_SIZE];
        memset(&output, 0, BUFFER_SIZE);
        int status, byte_read;

        close(pipe_fd[WRITE_END]);

        byte_read = read(pipe_fd[READ_END], output, sizeof(output));
        printf("Byte read was: %d\n", byte_read);

        wait(&status);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            int exit_status = WEXITSTATUS(status);
            printf("Output was: %s\n", output);
            printf("ack exited with status %d\n", exit_status);
            if (exit_status == 0)
            {
                printf("Request accepted.\n");
            }
            else
            {
                printf("Request rejected.\n");
            }
        }
    }
    return 0;
}
