#include <stdio.h>
#include <errno.h>
#include <mqueue.h> // Thư viện Linux POSIX Message Queues
#include <fcntl.h>  // Thư viện cung cấp các cờ (O_RDONLY,...)
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
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
    server_attr.mq_flags = 0;
    server_attr.mq_maxmsg = 10;
    server_attr.mq_msgsize = BUFFER;
    server_attr.mq_curmsgs = 0;

    char client_str[BUFFER];
    unsigned int client_prio;

    mqd_t server_des  = mq_open("/server1", O_RDWR | O_CREAT, MQ_MODE, &server_attr);
    if (server_des == -1)
    {
        handle_error("mq_open");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
    ssize_t server_receive = mq_receive(server_des, client_str, sizeof(client_str), &client_prio);
    if (server_receive == -1)
    {
        handle_error("mq_receive");
        exit(EXIT_FAILURE);
    }
    else if(server_receive > 0)
    {

        printf("%s\n", client_str);
        sleep(1);
        printf("%ld\n", server_receive);

    }

    else if(server_receive == 0){
        break;
    }
    }
    
    mq_unlink ("/server1");
    return 0;
}