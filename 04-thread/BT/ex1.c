

/* Viết một chương trình ghi 100000 kí tự A vào file test.txt
và tính toán thời gian thực hiện xong công việc này thông qua
việc sử dụng lệnh time. (Lưu ý: chỉ dùng duy nhất main thread)*/

/*HW*/
/*- Sử dụng cấp phát động để cấp phát 1 chuỗi nhập vào
  - Sử dụng systemcall open, write, close để ghi 100000 lần chuỗi vào file
  - Sử dụng main thread để chạy làm write*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

char *dynamicString();
void *writetxt(void *arg); // Đã chỉnh sửa tên hàm và khai báo đúng

// Cấu trúc để lưu trữ thời gian bắt đầu và kết thúc của thread
typedef struct
{
    struct timeval start_time;
    struct timeval end_time;
} ThreadTiming;

/*Hàm main*/
int main(int argc, char *argv[])
{
    ThreadTiming timing; // Khởi tạo biến timing kiểu ThreadTiming
    // pthread_self lấy id của thread hiện tại (tức à thread main)
    pthread_t mainThread = pthread_self();
    // khởi tạo thread với id mainThread (id của thread main)
    // writetxt truyền vào hàm thực thi trong thread
    // (void*)&timing ép kiểu biến timing về kiểu void và truyền vào hàm write
    int ret = pthread_create(&mainThread, NULL, writetxt, (void *)&timing);
    if (ret != 0)
    {
        printf("Failed to create thread\n");
        return 1;
    }
    pthread_join(mainThread, NULL); // Đợi thread kết thúc
    // Lấy giá trị thời gian bắt đầu từ biến timing.start_time.tv_sec
    long long start_microseconds = timing.start_time.tv_usec + timing.start_time.tv_usec;
    // Lấy giá trị thời gian bắt đầu từ biến timing.end_time.tv_sec
    long long end_microseconds = timing.end_time.tv_usec + timing.end_time.tv_usec;
    // Tính thời gian thread chạy
    long long elapsed_microseconds = end_microseconds - start_microseconds;
    printf("Total time thread: %lld uS\n ", elapsed_microseconds);
    return 0;
}
/*Hàm cấp phát động cho chuỗi*/
char *dynamicString()
{
    // Cấp phát động cho chuỗi
    char *str = (char *)malloc(sizeof(char));
    if (str == NULL)
    {
        printf("Not enough memory!\n");
        return NULL;
    }
    // khởi tạo biến capacity:Dung tích chuỗi,length: độ dài chuỗi
    int capacity = 1, length = 0;
    char c;
    // Đọc dữ từ bàn phí cho tới khi gặp kí tự '\n'
    while ((c = getchar()) != '\n')
    {
        // gán kí tự và chuỗi
        str[length] = c;
        // Tăng biến độ dài lên 1
        length++;
        // Kiểm tra độ dài cuổi chuỗi so với dung tích của chuỗi
        // Nếu thiếu tiến hành cấp phát x2 bộ nhớ cho chuỗi
        if (length == capacity)
        {
            capacity *= 2;
            // tạo ra con trỏ temp để cấp phát, khi lỗi cấp phái vẫn giữ được
            // nguyên vẹn chuỗi str
            char *temp = (char *)realloc(str, capacity * sizeof(char));
            if (temp == NULL)
            {
                printf("Not enough memory!\n");
                free(str);
                return NULL;
            }
            //
            str = temp;
        }
    }
    str[length] = '\0';

    return str; // Trả về con trỏ chuỗi đã cấp phát
}
/*Hàm thực hiện ghi 100000 vào file text.txt*/
void *writetxt(void *arg)
{
    printf("Import string: \n");
    char *str = dynamicString();
    // ép kiểu con trỏ arg về kiểu ThreadTiming
    ThreadTiming *timing = (ThreadTiming *)arg;
    // Ghi lại thời gian bắt đầu của thread
    gettimeofday(&(timing->start_time), NULL);
    // Dùng systemcall open mở file, O_RDWR cấp quyền đọc viết,
    // O_CREAT nếu không file sẽ tạo file text.txt, 0667 cấp quyền cho file khi tạo mới
    int op = open("text.txt", O_RDWR | O_CREAT, 0667);
    if (op == -1)
    {
        printf("Cannot open text.txt\n");
        return NULL;
    }
    // Ghi 100000 lần vào file
    for (int i = 1; i < 20; i++)
    {

        ssize_t wr = write(op, str, strlen(str));
        if (wr == -1)
        {
            printf("Failed to write to file\n");
        }
    }
    // Đóng file
    close(op);
    /* Thu hồi bộ nhớ đã cấp phát của con trỏ str
    đc trỏ đến địa chỉ của con trỏ str( trong hàm dynamicString)*/
    free(str);
    // Ghi lại thời gian kết thúc của thread
    gettimeofday(&(timing->end_time), NULL);
    return NULL;
}
/*Mặc dù đề bài không yêu cấp phát động cho 1 chuỗi nhập vào.
  Mục đích thêm vào là để ôn tập cấp phát động.*/