
/*Creat thread write to deveci driver*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
void *write_thread(void *arg);
char *str = "Hello my devices 12345"; /*String will write to device*/
int main(int agc, char arvg[])
{
    pthread_t mainThread = pthread_self();                           // Take number mainthread
    int ret = pthread_create(&mainThread, NULL, write_thread, NULL); // creat thread
    if (ret != 0)
    {
        printf("Failed to create thread\n");
        return 1;
    }
    pthread_join(mainThread, NULL); // Wait thread end
    return 0;
}
// Function  the thread will call
void *write_thread(void *arg)
{
    // system call open and licensing read, write for thread
    int op = open("/dev/my_device", O_RDWR);
    if (op == -1)
    {
        printf("Can not opent /dev/my_device");
        return NULL;
    }
    ssize_t wr = write(op, str, strlen(str)); // system call write
    if (wr == -1)
    {
        printf("Failed to write to file\n");
    }

    close(op); // system call close
    return NULL;
}