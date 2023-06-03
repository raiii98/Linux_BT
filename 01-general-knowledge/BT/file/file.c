#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd = open("filesystem.txt", O_WRONLY | O_APPEND); // O_APPEND tự đông đặt con trỏ cuối files
    if (fd == -1)
    {
        // Xử lý lỗi khi mở tệp tin
        printf(" open call system failed");
    }

    // Ghi dữ liệu vào tệp tin
    write(fd, "Hello, World!\n", 14);

    // Đóng tệp tin
    close(fd);

    return 0;
}
