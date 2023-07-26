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

    struct mq_attr client_attr;
}