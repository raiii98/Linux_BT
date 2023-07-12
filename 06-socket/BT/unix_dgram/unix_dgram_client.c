#include <sys/un.h>
#include <sys/socket.h>
#include <stddef.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)
#define BUF_SIZE 256
#define SOCK_PATH "./sock_dgram1"

int main(int argc, char *argv[])
{
    struct sockaddr_un server_add;
    int client_fd;
    int opt = 1;
    size_t msgLen;
    ssize_t numBytes;
    char trans[BUF_SIZE];

    client_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (client_fd == -1)
    {
        handle_error("socket()");
        exit(EXIT_FAILURE);
    }
    memset(&server_add, 0, sizeof(struct sockaddr_un));
    memset(&trans, 0, sizeof(trans));

    server_add.sun_family = AF_UNIX;
    strncpy(server_add.sun_path, SOCK_PATH, sizeof(server_add.sun_path) - 1);
    setsockopt(client_fd, SOL_SOCKET, SO_PASSCRED, &opt, sizeof(opt));
    msgLen = strlen(argv[1]);
    if (sendto(client_fd, argv[1], msgLen, 0, (struct sockaddr *)&server_add, sizeof(struct sockaddr_un)) == -1)
    {
        handle_error("sendto()");
        exit(EXIT_FAILURE);
    }
    numBytes = recvfrom(client_fd, trans, BUF_SIZE, 0, NULL, NULL);
    if (numBytes == -1)
    {

        handle_error("recvfrom()");
        exit(EXIT_FAILURE);
    }
    else
        printf("Response : %s\n", trans);

    return 0;
}