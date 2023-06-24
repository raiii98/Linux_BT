
/*Creat thread write to deveci driver*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024
char *str = "Hello my devices 1234"; /*String will write to device*/
char buffer[BUFFER_SIZE];

int main(int agc, char arvg[])
{
    // system call open and licensing read, write for thread
    int op = open("/dev/my_device", O_RDWR);
    if (op == -1)
    {
        printf("Can not opent /dev/my_device");
    }
    ssize_t wr = write(op, str, strlen(str)); // system call write
    if (wr == -1)
    {
        printf("Failed to write to file\n");
    }
    ssize_t re = read(op, buffer, sizeof(buffer)); // system call read.
    if (re == -1)
    {
        printf("Failed to read file");
        close(op); // system call close
    }

    printf("Read %zd bytes: %s\n", re, buffer);
    close(op); // system call close

    return 0;
}
// Function  the thread will call
