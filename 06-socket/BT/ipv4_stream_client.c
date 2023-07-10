#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h> //  Chứa cấu trúc cần thiết cho socket.
#include <netinet/in.h> //  Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

void chat_function(int client_fd)
{
    int numb_write, numb_read;
    char rec_buff[BUFFER_SIZE];
    char trans_buff[BUFFER_SIZE];
    while (1)
    {
        memset(trans_buff, '0', sizeof(trans_buff));
        memset(rec_buff, '0', sizeof(trans_buff));
        printf("Please enter the message: ");
        fgets(trans_buff, sizeof(trans_buff), stdin);

        numb_write = write(client_fd, trans_buff, sizeof(trans_buff));
        if (numb_write == -1)
        {
            handle_error("write()");
        }
        if (strncmp("exit", rec_buff, 4) == 0)
        {
            printf("Server exit ...\n");
            break;
        }
        printf("\nMessage from Server: %s\n", rec_buff);
    }
    close(client_fd); /*close*/
}

int main(int argc, char *argv[])
{
    int port_numb;
    int client_fd;
    struct sockaddr_in client_add;
    memset(&client_add, 0, sizeof(client_add));

    if (argc < 3)
    {
        printf("command : ./client <server address> <port number>\n");
        exit(EXIT_FAILURE);
    }
    port_numb = atoi(argv[2]);
    client_add.sin_port = htons(port_numb);
    client_add.sin_family = AF_INET;
    if (inet_pton(AF_INET, argv[1], &client_add.sin_addr) == -1)
    {
        handle_error("inet_pton()");
    }
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1)
    {
        handle_error("socket()");
    }
    if (connect(client_fd, (struct sockaddr *)&client_add, sizeof(client_add)) == -1)
    {
        handle_error("connect()");
    }
    chat_function(client_fd);

    return 0;
}