#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

char *dynamicString();
void *writetxt(void *arg); // Đã chỉnh sửa tên hàm và khai báo đúng

int main(int argc, char *argv[]) // Đã sửa khai báo đúng
{
    pthread_t mainThread = pthread_self();
    int ret = pthread_create(&mainThread, NULL, writetxt, NULL);
    if (ret != 0)
    {
        printf("Failed to create thread\n");
        return 1;
    }
    pthread_join(mainThread, NULL); // Đợi thread kết thúc

    return 0;
}

char *dynamicString()
{
    char *str = (char *)malloc(sizeof(char));
    if (str == NULL)
    {
        printf("Not enough memory!\n");
        return NULL;
    }

    int capacity = 1, length = 0;
    char c;

    while ((c = getchar()) != '\n')
    {
        str[length] = c;
        length++;
        if (length == capacity)
        {
            capacity *= 2;
            char *temp = (char *)realloc(str, capacity * sizeof(char));
            if (temp == NULL)
            {
                printf("Not enough memory!\n");
                free(str);
                return NULL; // Đã sửa để trả về NULL nếu không đủ bộ nhớ
            }
            str = temp;
        }
    }
    str[length] = '\0';

    return str; // Trả về con trỏ chuỗi đã cấp phát
}

void *writetxt(void *arg)
{
    printf("Import string: \n");
    char *str = dynamicString();

    int op = open("text.txt", O_RDWR | O_CREAT, 0667);
    if (op == -1)
    {
        printf("Cannot open text.txt\n");
        return NULL;
    }
    for (int i = 0; i < 100000; i++)
    {

        ssize_t wr = write(op, str, strlen(str));
        if (wr == -1)
        {
            printf("Failed to write to file\n");
        }
    }

    close(op);
    free(str);

    return NULL;
}
