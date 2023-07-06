#include<stdio.h>

#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

int main(void)
{
       long position = 0;
       int fd = 0;
       char buf[] = "Hello, this is thevngeeks";
       char buf_read[8];
       char buf_read1[32];
       int length = 0;

       /*Mở một file hello.txt với cờ O_RDWR, ghi string buf[] vào file đó*/
       fd = open("hello.txt", O_RDWR|O_CREAT);
       if(fd == -1)
       {
           return -1;
       }
       length = write(fd, buf, sizeof(buf));

       /*dùng lseek() với đối số SEEK_CUR để xem giá trị file offset hiện tại*/
       position = lseek(fd, 0, SEEK_CUR);
       printf("Current position:%ld\n", position);

       /*lseek()với đối số SEEK_SET để trỏ về đầu file*/
       position = lseek(fd, 0, SEEK_SET);

       /*Dùng read() để đọc file, lưu vào string buf_read.
         String đọc ra sẽ là buf[]: Hello, this is thevngeeks*/

       memset(buf_read, 0x0, sizeof(buf_read));
       length = read(fd, buf_read, sizeof(buf_read));
       buf_read[length] = '\0';
       printf("buf_read:%s\n", buf_read);

       /*File offset hiện tại là bao nhiêu sau read()?*/
       position = lseek(fd, 0, SEEK_CUR);
       printf("current position:%ld\n", position);
       /*Dùng write() ghi chuỗi “HELLO” vào file từ vị trí file offset*/
       write(fd, "HELLO",5);
       /*Trở về đầu file để đọc nội dung file*/
       position = lseek(fd, 0, SEEK_SET);

       /*Nội dung file sau khi đã ghi đè chuỗi “HELLO” vào file từ vị trí offset*/
       memset(buf_read1, 0x0, sizeof(buf_read1));
       length = read(fd, buf_read1, sizeof(buf_read1));
       buf_read1[length] = '\0';
       printf("buf_read:%s\n", buf_read1);

       /*Đóng file hello.txt bằng mô tả file fd*/
       close(fd);

       return 0;

}