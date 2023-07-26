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
    char ptr[]= "Hello my client 1\n";
    struct mq_attr client_attr;
    client_attr.mq_flags = 0;
    client_attr.mq_maxmsg = 10;
    client_attr.mq_msgsize = BUFFER;
    client_attr.mq_curmsgs = 0;
    unsigned int client1_prio = 1;
    mqd_t client_des = mq_open("/server1", O_RDWR | O_CREAT, MQ_MODE, &client_attr);

    int client_send= mq_send(client_des,ptr,strlen(ptr),client1_prio);
    if(client_send==-1){
        handle_error("mq_send");
        exit(EXIT_FAILURE);

    }
    return 0;
}