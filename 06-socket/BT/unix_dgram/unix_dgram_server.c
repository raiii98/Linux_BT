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
    struct sockaddr_un server_add, client_add;
    int fd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];
    int opt = 1;

    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd == -1)
    {
        handle_error("socket()");
        exit(EXIT_FAILURE);
    }

    memset(&server_add, 0, sizeof(struct sockaddr_un));
    server_add.sun_family = AF_UNIX;
    strncpy(server_add.sun_path, SOCK_PATH, sizeof(server_add.sun_path) - 1);
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        handle_error("setsockotp()");
    }
    if (bind(fd, (struct sockaddr *)&server_add, sizeof(struct sockaddr_un)) == -1)
    {
        handle_error("bind()");
        exit(EXIT_FAILURE);
    }
    printf("start listening on server\n");

    while (1)
    {
        len = sizeof(struct sockaddr_un);
        numBytes = recvfrom(fd, buf, BUF_SIZE, 0, (struct sockaddr *)&client_add, &len);
        if (numBytes == -1)
        {
            handle_error("recvfrom()");
            exit(EXIT_FAILURE);
        }

        printf("Server received %ld bytes from %s \n", (long)numBytes, client_add.sun_path);

        for (j = 0; j < numBytes; j++)
            buf[j] = toupper((unsigned char)buf[j]);

        if (sendto(fd, buf, numBytes, 0, (struct sockaddr *)&client_add, len) != numBytes)
        {
            handle_error("sendto()");
            exit(EXIT_FAILURE);
        }
    }
}
