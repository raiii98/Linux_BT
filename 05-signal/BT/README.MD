# Realtime signal và standard signal là gì? Phân biệt sự khác nhau giữa chúng.
### Real-time signal:
- Real-time signal trong Linux là một dạng tín hiệu đặc biệt được sử dụng để truyền thông với các tiến trình và luồng (threads) trong hệ thống thời gian thực.
- Tín hiệu thời gian thực được ưu tiên cao hơn các tín hiệu thông thường (**một số tài liệu có viết giữa Real-time signal và Standard signal không xác định độ ưu tiên**) và có thể được ứng dụng để đồng bộ và xử lý các tác vụ theo thời gian thực như điều khiển, đồng bộ hóa, hoặc xử lý tín hiệu số. 
- Các tín hiệu thời gian thực trong Linux bao gồm SIGRTMIN, SIGRTMIN+1, ..., SIGRTMAX.
### Standard signal
- Standard signal trong Linux là các tín hiệu thông thường được sử dụng để giao tiếp với các tiến trình và luồng trong hệ thống.
- Các tín hiệu thông thường thường được sử dụng để thông báo về các sự kiện như kết thúc tiến trình, yêu cầu kết thúc tiến trình, lỗi xảy ra, hoặc tương tác người dùng.
- Các tín hiệu thông thường trong Linux bao gồm SIGINT (tín hiệu ngắt từ bàn phím), SIGTERM (yêu cầu kết thúc tiến trình), SIGSEGV (lỗi phân đoạn), SIGILL (lỗi chỉ thị), và nhiều tín hiệu khác.
------------
### Phân biệt Real-time signal và Standard signal:
>Kiểm tra bằng code trên ubuntu khi dùng hàm `sigqueue` thì Real-time signal và Standard signal đều gửi kèm giá trị được.
>- Nếu một tín hiệu thời gian thực (Real-time signal) được gửi bằng hàm `sigqueue`, một giá trị hoặc một con trỏ có thể được gửi cùng với tín hiệu. Giá trị này hoặc con trỏ có thể được lấy ra bởi quá trình nhận từ tham số thứ hai của bộ xử lý tín hiệu thời gian thực, con trỏ tới siginfo_t. Giá trị hoặc con trỏ được lưu lần lượt trong các thành viên si_value hoặc si_ptr.
- Tín hiệu thời gian thực được xếp hàng đợi cho quá trình nhận. Điều này áp dụng ngay cả khi nhận được cùng một tín hiệu thời gian thực ( Real-time signal ) nhiều lần. Ngược lại, nếu một tín hiệu tiêu chuẩn ( Standard signal ) bị chặn và nhiều phiên bản của tín hiệu đó được gửi đến một quy trình, thì chỉ một phiên bản đang chờ xử lý và phần còn lại sẽ bị loại bỏ.
- Nếu nhiều tín hiệu thời gian thực được xếp vào hàng đợi cho một quy trình, chúng sẽ được phân phối theo thứ tự tăng dần của số tín hiệu, tức là tín hiệu thời gian thực thấp hơn trước. Ngược lại, thứ tự phân phối của nhiều tín hiệu tiêu chuẩn được xếp hàng đợi là không xác định. Nếu nhiều phiên bản của cùng một tín hiệu thời gian thực được xếp vào hàng đợi cho một quy trình, chúng sẽ được phân phối theo thứ tự thời gian. Nếu cả tín hiệu tiêu chuẩn và thời gian thực được xếp vào hàng đợi cho một quy trình, tín hiệu tiêu chuẩn sẽ được phân phối trước ( `Do giữa Real-time signal và Standard signal không xác định độ ưu tiên, nên có thể phụ thuộc vào loại hệ điều hành sẽ quyết định loại tín hiệu nào được ưu tiên. Trong bài code bên dưới code trên hệ điều hành ubuntu thì Real-time signal được ưu tiên` ). Điều này phù hợp với khái niệm rằng các tín hiệu được đánh số thấp hơn được phân phối trước.
------------
[Link bài code](https://github.com/raiii98/Linux_BT/blob/master/05-signal/BT/ex2.c "Link bài code")
------------
Trên cửa sổ console:
- B1: gcc -o example ex2.c
- B2: ./example