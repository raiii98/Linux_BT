#include <stdio.h>
#include <errno.h>
#include <mqueue.h> // Thư viện Linux POSIX Message Queues
#include <fcntl.h>  // Thư viện cung cấp các cờ (O_RDONLY,...)
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUFFER 256
#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

#define MQ_MODE (S_IRUSR | S_IWUSR)

int main(int argc, char *argv[])
{
    char ptr1[] = "Client1.Hello my name is client 1\n";
    char ptr2[] = "Client2.Hello my name is client 2\n";

    struct mq_attr client_attr;
    client_attr.mq_flags = 0;
    client_attr.mq_maxmsg = 10;
    client_attr.mq_msgsize = BUFFER;
    client_attr.mq_curmsgs = 0;
    unsigned int client1_prio = 1, client2_prio = 2;
    mqd_t client_des = mq_open("/server1", O_RDWR | O_CREAT, MQ_MODE, &client_attr);
    pid_t client1_pid = fork();
    if (client1_pid < 0)
    {
        handle_error("fork()");
        exit(EXIT_FAILURE);
    }
    else if (client1_pid == 0)
    {
        int client1_send = mq_send(client_des, ptr1, strlen(ptr1), client1_prio);
        if (client1_send == -1)
        {
            handle_error("mq_send");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        wait(NULL);
        int client2_send = mq_send(client_des, ptr2, strlen(ptr2), client2_prio);
        if (client2_send == -1)
        {
            handle_error("mq_send");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}