#include <stdio.h>
#include <errno.h>
#include <mqueue.h> // Thư viện Linux POSIX Message Queues
#include <fcntl.h>  // Thư viện cung cấp các cờ (O_RDONLY,...)
#include <string.h>
#include <stdlib.h>

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

    struct mq_attr server_attr;
    char client_str[BUFFER];
    unsigned int client_prio;
    mqd_t server_des = mq_open("/server", O_RDWR | O_CREAT, MQ_MODE, server_attr);
    if (server_des == -1)
    {
        handle_error("mq_open");
        exit(EXIT_FAILURE);
    }

    if (mq_receive(server_des, client_str, sizeof(client_str), &client_prio) == -1)
    {
        handle_error("mq_receive");
        exit(EXIT_FAILURE);
    }
    else
    {

        printf("%s", client_str);
        sleep(1);
    }
    return 0;
}