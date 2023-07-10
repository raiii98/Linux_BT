#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#define LISTEN_BACKLOGS 10
#define BUFFER_SIZE 256

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

void chat_function(int new_socket_fd)
{

    int num_read, num_write;
    char trans_buff[BUFFER_SIZE], rec_buff[BUFFER_SIZE];

    while (1)
    {
        memset(trans_buff, '0', sizeof(trans_buff));
        memset(rec_buff, '0', sizeof(rec_buff));
        num_read = read(new_socket_fd, rec_buff, sizeof(rec_buff));
        if (num_read == -1)
        {
            handle_error("read()");
        }
        if (strncmp("exit", rec_buff, 4))
        {
            system("clear");
            break;
        }
        printf("\nMessage from Client: %s\n", rec_buff);

        printf("Please respond the message: ");
        fgets(trans_buff, sizeof(trans_buff), stdin);
        num_write = write(new_socket_fd, trans_buff, sizeof(trans_buff));
        if (num_write == -1)
        {
            handle_error("write()");
        }
        if (strncmp("exit", trans_buff, 4))
        {
            system("clear");
            break;
        }
        sleep(1);
    }
    close(new_socket_fd);
}

int main(int argc, char *argv[])
{

    int port_numb, len, opt = 1;
    int server_fd, new_socket_fd;
    struct sockaddr_in server_add, client_add;

    if (argc < 2)
    {
        printf("No port provided\ncommand: ./server <port number>\n");
        exit(EXIT_FAILURE);
    }
    else
        port_numb = atoi(argv[1]);
    memset(&server_add, '0', sizeof(struct sockaddr_in));
    memset(&client_add, '0', sizeof(struct sockaddr_in));

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        handle_error("socket()");
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        handle_error("setsockotp()");
    }
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(port_numb);
    server_add.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (struct sockaddr *)&server_add, sizeof(server_add)) == -1)
    {
        handle_error("bind()");
    }
    if (listen(server_fd, LISTEN_BACKLOGS) == -1)
    {
        handle_error("listen()");
    }
    len = sizeof(client_add);
    while (1)
    {
        printf("Server is listening at port : %d \n....\n", port_numb);
        new_socket_fd = accept(server_fd, (struct sockaddr *)&client_add, (socklen_t *)&len);
        if (new_socket_fd == -1)
        {
            handle_error("accep()");
        }
        system("clear");
        printf("Server : got connection \n");
        chat_function(new_socket_fd);
    }

    close(server_fd);
    return 0;
}
