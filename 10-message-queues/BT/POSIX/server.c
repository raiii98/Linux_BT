#include <stdio.h>
#include <errno.h>
#include <mqueue.h> // Thư viện Linux POSIX Message Queues
#include <fcntl.h>  // Thư viện cung cấp các cờ (O_RDONLY,...)
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER 256

#define Client1_Header "Client1"
#define Client2_Header "Client2"

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
    server_attr.mq_flags = 0;
    server_attr.mq_maxmsg = 10;
    server_attr.mq_msgsize = BUFFER;
    server_attr.mq_curmsgs = 0;

    char client_str[BUFFER];
    unsigned int client_prio;
    memset(client_str, 0, BUFFER);
    mqd_t server_des = mq_open("/server1", O_RDWR | O_CREAT, MQ_MODE, &server_attr);
    if (server_des == -1)
    {
        handle_error("mq_open");
        exit(EXIT_FAILURE);
    }
    while (1)
    {

        memset(client_str, 0, BUFFER);
        ssize_t server_receive = mq_receive(server_des, client_str, sizeof(client_str), &client_prio);
        if (server_receive == -1)
        {
            handle_error("mq_receive");
            exit(EXIT_FAILURE);
        }
        else if (server_receive > 0)
        {
            if (strncmp("Client1", client_str, 7) == 0)
            {
                char *new_str = client_str + 8;
                printf("Messages received from Client 1: %s\n", new_str);
            }
            else if (strncmp(Client2_Header, client_str, 7) == 0)
            {

                char *new_str = client_str + 8;
                printf("Messages received from Client 2: %s\n", new_str);
            }

            sleep(1);
        }

        else if (server_receive == 0)
        {
            break;
        }
    }

    mq_unlink("/server1");
    return 0;
}