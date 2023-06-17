
/*Viết một chương trình ghi 100000 kí tự A vào file test.txt
 và tính toán thời gian thực hiện xong công việc này thông qua
  việc sử dụng lệnh time. (Lưu ý: Dùng tối đa 10 thread để ghi)*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#define NUM_THREADS 10
// Khai báo struct timing
typedef struct
{
    struct timeval start_time;
    struct timeval end_time;
} ThreadTiming;
// Khai báo struct truyền vào thread, vì thread chỉ cho phép truyền 1 đối số
typedef struct
{
    ThreadTiming timing;
    char *thread_str;
} ThreadArgs;
// Khai báo mutex
pthread_mutex_t mutex; // Biến Mutex
// Khai báo biến tính tổng thời gian thread hoạt động
long long totaltime;

char *dynamicString();
void *writetxt(void *arg);

/*************************************
 *               Hàm main            *
 *************************************/

int main(int argc, char *argv[])
{
    totaltime = 0;
    ThreadArgs Args[NUM_THREADS];
    pthread_t threads[NUM_THREADS];

    printf("Import string:\n");
    char *str = dynamicString();

    // Khởi tạo Mutex
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        Args[i].thread_str = strdup(str); // Sao chép chuỗi
        int ret = pthread_create(&threads[i], NULL, writetxt, &Args[i]);
        if (ret != 0)
        {
            printf("Failed to create thread\n");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL); // Đợi tất cả các luồng kết thúc
    }

    // Giải phóng bộ nhớ trong luồng chính
    for (int i = 0; i < NUM_THREADS; i++)
    {
        free(Args[i].thread_str);
    }

    pthread_mutex_destroy(&mutex); // Giải phóng Mutex

    printf("Total time thread: %lld uS\n", totaltime);
    free(str);
    return 0;
}

/*************************************
 *    Hàm cấp phát động string       *
 *************************************/
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

/*************************************
 *               Hàm thread          *
 *************************************/

void *writetxt(void *arg)
{
    // Ép kiểu từ void về ThreadArgs
    ThreadArgs *Args = (ThreadArgs *)arg;
    gettimeofday(&Args->timing.start_time, NULL);

    int op = open("text.txt", O_RDWR | O_CREAT, 0667);
    if (op == -1)
    {
        printf("Cannot open test.txt\n");
        return NULL;
    }

    for (int i = 0; i < 10000; i++)
    {
        // Khoá mutex
        pthread_mutex_lock(&mutex); // Khóa Mutex trước khi ghi vào file

        ssize_t wr = write(op, Args->thread_str, strlen(Args->thread_str));
        if (wr == -1)
        {
            printf("Failed to write to file\n");
        }
        // Mở khoá mutex
        pthread_mutex_unlock(&mutex); // Mở khóa Mutex sau khi ghi vào file
    }

    close(op);
    gettimeofday(&Args->timing.end_time, NULL);
    // Lấy giá trị thời gian bắt đầu từ biến timing.start_time.tv_sec
    long long start_microseconds = Args->timing.start_time.tv_usec + Args->timing.start_time.tv_usec;
    // Lấy giá trị thời gian bắt đầu từ biến timing.end_time.tv_sec
    long long end_microseconds = Args->timing.end_time.tv_usec + Args->timing.end_time.tv_usec;
    // Tính thời gian thread chạy
    long long elapsed_microseconds = end_microseconds - start_microseconds;
    // Tính tổng cộng dồn thời gian các thread bằng hàng tính đồng bộ __sync_add_and_fetch
    __sync_add_and_fetch(&totaltime, elapsed_microseconds);
    // Kết thúc thread
    pthread_exit(NULL);
}
