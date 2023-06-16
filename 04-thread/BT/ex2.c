#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 10
#define NUM_CHARS 2
#define FILENAME "text.txt"
#define CHAR_TO_WRITE 'A'

void *write_to_file(void *arg)
{
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL)
    {
        perror("Cannot open file");
        pthread_exit(NULL);
    }

    for (int i = 0; i < NUM_CHARS; i++)
    {
        fputc(CHAR_TO_WRITE, file);
    }

    fclose(file);
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int rc;

    // Ghi thời gian bắt đầu
    time_t start_time = time(NULL);

    // Tạo các luồng và thực hiện ghi vào file
    for (int i = 0; i < NUM_THREADS; i++)
    {
        rc = pthread_create(&threads[i], NULL, write_to_file, NULL);
        if (rc)
        {
            perror("Cannot create thread");
            exit(-1);
        }
    }

    // Chờ tất cả các luồng hoàn thành công việc
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Tính thời gian kết thúc và in ra màn hình
    time_t end_time = time(NULL);
    double execution_time = difftime(end_time, start_time);
    printf("Execution time: %.6f seconds\n", execution_time);

    return 0;
}
