# ✈️ Kiến trúc và Luồng Thực Thi của Dự án Đặt Vé ITF-Airway

Dự án **ITF-Airway** là một ứng dụng Console (command-line application) được viết bằng C++ với mục tiêu số hóa quy trình check-in và đặt chỗ máy bay. Hệ thống được tổ chức hoàn toàn theo trường phái Lập trình Hướng đối tượng (OOP) cơ bản, không sử dụng các container có sẵn của C++ Standard Library (như `std::vector`), mà sử dụng cấp phát bộ nhớ động thông qua con trỏ và mảng song song để quản lý dữ liệu.

---

## 1. Cấu trúc Dự Án và Các Lớp (Architecture)

Dự án tuân thủ nghiêm ngặt việc tách biệt giữa khai báo và định nghĩa hàm (tách file `.h` và `.cpp`), bao gồm 4 thành phần thiết yếu sau (từ mức trừu tượng cao nhất đến thấp nhất):

### 1.1 Điểm khởi chạy: `main.cpp`
Đây là bộ khung bên ngoài cùng của chương trình, đóng vai trò tạo vòng đời ứng dụng với 3 bước rất rõ ràng: 
* Khởi động chương trình (`startup`)
* Tiến hành chạy vòng lặp menu (`run`)
* Đóng gói dọn dẹp hệ thống (`shutdown`)

### 1.2 Lớp Điều khiển Vòng Đời: `Application`
Lớp `Application` đảm nhận nhiệm vụ giao tiếp trực tiếp với người dùng và điều phối các luồng xử lý:
* **Tương tác In/Out:** In ra các menu, nhận đầu vào action code (`Open`, `Close`, `Exit`).
* **Quản lý luồng Booking:** Thông qua vòng lặp `processBookingLoop()`, lớp này điều hướng các lệnh nhập số ghế từ người dùng về cho `FlightManager`.
* **Vòng đời:** Sở hữu bên trong là 1 biến lưu trạng thái `isRunning` và một con trỏ kiểu `FlightManager`. Khi ứng dụng tắt đi (hàm `shutdown`), nó gọi việc dọn dẹp các luồng dữ liệu đang lỡ dở.

### 1.3 Lớp Quản Lý Chuyến Bay: `FlightManager`
Lớp này nằm giữa `Application` và `Flight`, có trách nhiệm theo dõi và quản lý dữ liệu File.
* **Quản lý trạng thái:** Trực tiếp lưu giữ một con trỏ đến chuyến bay hiện tại (`Flight* currentFlight`).
* **Xử lý File:** Nhận cấu hình tên file Input (chứa trạng thái ghế `seats_input.txt`) và file Output (chứa thông tin đặt vé `booking_output.txt`).
* **Hoạt động:** Khi lệnh mở hoặc đóng chuyến bay được đưa ra, `FlightManager` là lớp chịu trách nhiệm cấp phát mới (với từ khoá `new`) hay thu hồi (`delete`) đối tượng chuyến bay. Đồng thời kiêm nhiệm in ra danh sách ánh xạ cuối cùng qua hành động `displayFinalMapping()`.

### 1.4 Lớp Đối Tượng Nghiệp Vụ Chính: `Flight`
Đây là lớp lõi thực hiện các thuật toán tính toán và thay đổi dữ liệu bên trong cấu trúc mảng.
* **Biến trạng thái:** Lưu trữ `flightCode` (Mã chuyến), `maxSeats` (Tổng ghế) và `availableSeats` (Số ghế trống).
* **Cấu trúc Dữ liệu:** Để quản lý thông tin ghế, lớp sử dụng 3 mảng động (được cấp phát bên trong Constructor dựa vào số ghế tối đa):
  * `int* seatStatus`: Đánh dấu tình trạng một ghế cụ thể là 'Trống' (0) hay 'Đã đặt' (1).
  * `std::string* ticketCodes`: Lưu trữ mã vé do người dùng nhập.
  * `int* allocatedSeats`: Lưu số ghế thực tế ánh xạ ứng với mã vé tương ứng. 
  *(Thiết kế này gọi là **Parallel Arrays** - Mảng song song).*
* **Hành vi xử lý:** Khôi phục ghế từ CSDL file (hàm `loadInitialSeats`), cũng như xử lý logic lựa chọn ghế, lưu trữ theo thiết kế append thẳng xuống file output (`bookSeats`).

---

## 2. Luồng Thực Thi Chi Tiết (Execution Flow)

Chương trình hoạt động theo một quy trình tuyến tính với các "trạng thái kịch bản" tương ứng. Dưới đây là flow chi tiết:

### Bước 1: Khởi động hệ thống (System Startup)
1. Khi chương trình bắt đầu, hàm `main()` khởi tạo đối tượng `Application` và gọi `.startup()`.
2. `Application` in ra banner chào mừng của ITF-Airway.
3. Bộ cấu hình tên file được điền sẵn: input là `seats_input.txt`, output là `booking_output.txt`.
4. Khởi tạo đối tượng `FlightManager` và bật vòng lặp `isRunning = true`.

### Bước 2: Vòng lặp chờ lệnh (Main Action Loop)
* Chương trình dừng lại và chờ Terminal nhập Command (`Open`, `Close`, hoặc `Exit`).

### Bước 3: Kịch bản `Open` - Mở Quầy Check-in
Nếu nhập `Open`:
1. `Application` sẽ nhờ `FlightManager` kiểm tra xem đã có chuyến bay nào đang mở hay chưa. Nếu có rồi, báo lỗi và bắt buộc phải `Close` trước.
2. Nếu trạng thái hợp lệ (chưa mở), yêu cầu người dùng nhập Mã Chuyến Bay (VD: *VJ123*).
3. `Application` ra lệnh cho `FlightManager` mở chuyến bay với số lượng 50 ghế (MAX_SEATS default).
4. `FlightManager` cấp phát bộ nhớ cho đối tượng `Flight` (ngầm khởi tạo 3 mảng động).
5. Sau khi mảng được sinh ra, gọi hàm `loadInitialSeats` mở chuỗi trạng thái từ `seats_input.txt`. 
6. `Flight` đếm các ID có giá trị bằng `1` để cập nhật biến `availableSeats` thật chuẩn xác.
7. Hiển thị thông tin hành trình (Flight Code, Total Seats, Available Seats).
8. Đưa luồng lập tức sang vòng lặp **Booking Loop**.

### Bước 4: Vòng lặp Booking - Đặt chỗ tự động (`processBookingLoop`)
Lúc này người dùng có thể thực hiện đăng ký liên tục mà không lo thoát luồng. Ở vòng lặp này:
1. Hỏi người dùng "Số lượng vé cần đặt cho đợt này" *(Nếu số lượng trống đạt giá trị `0`, không cho phép đặt thêm, bắt ép phải gõ chữ `Close` để đóng chuyến bay)*.
2. Khi người dùng nhập số > 0, hệ thống gọi hàm `Flight::bookSeats`:
   * Kiểm tra xem trong hệ thống còn đủ không gian cho số lượng vé yêu cầu không.
   * Lặp qua yêu cầu nhập Mã Vé (Ticket Code) cho **từng vị trí** mà người dùng muốn đặt.
   * **Xử lý tìm ghế:** Ứng dụng quét tuần tự mảng `seatStatus` từ vị trí đầu tiên lên. Nơi nào trạng thái ghế bằng `0`, nó sẽ đưa vào danh sách ghế tạm thời. Hệ thống tìm đủ số lượng thì dừng lại và show ra màn hình dải số ghế (VD: *"Gán tự động vào ghế 2, 4, 5"*).
   * Xác nhận Booking (Nhấn Y/N để đồng ý).
3. **Nếu nhấn Y:** Hệ thống dán nhãn `1` cho các ghế tại mảng `seatStatus`. Đồng thời cập nhật `ticketCodes`, `allocatedSeats`. Hệ thống ngay lập tức sinh file stream ở mode "Append" (`std::ios::app`) nhằm ghi trực tiếp 3 thông tin gồm: `Khóa chuyến bay | Mã Vé | ID Ghế` xuống disk (vào `booking_output.txt`).

### Bước 5: Kịch bản `Close` - Đóng Quầy Check-in
Nhiệm vụ này có thể gọi trực tiếp ở Menu chính (nếu đã từng mở chuyến) hoặc có thể gọi thông qua việc gõ lệnh `Close` ngay tại vòng lặp Booking số vé.
1. Hàm `Application::processCloseCommand()` và sau đó là `FlightManager::closeFlight()` được xử lí gọi.
2. In ra giao diện *"Đóng chức năng check-in"*.
3. Hàm `FlightManager::displayFinalMapping()` đọc file `booking_output.txt` quét xuống qua các dòng dữ liệu để in danh sách vé cuối cùng mà không cần truy xuất dữ liệu từ các vùng nhớ khác.
4. **Tuân thủ việc dọn rác:** Căn cứ vào tính chất memory-leak của C++, `FlightManager` thu hồi con trỏ `currentFlight` (gọi hàm `delete`). Ở chiều ngược lại, hàm Destructor của `Flight` sẽ đồng thời phá bỏ các con trỏ mảng (`delete[]`) của 3 cấu trúc mảng trên Heap.

### Bước 6: Kịch bản `Exit` - Shutdown an toàn
1. Khi có lệnh `Exit`, `isRunning` chuyển thành `false`, giải phóng chu trình `run()`.
2. Trả luồng về cho `main()`. `main()` sẽ yêu cầu gọi phương thức `shutdown()`.
3. `shutdown()` thông minh ở chỗ nó check nếu phát hiện vẫn còn `currentFlight` chưa kịp đóng, nó sẽ chạy tiếp luồng đóng tự động thay tay người thao tác.
4. Xóa đối tượng `FlightManager` khỏi chương trình, kết thúc triệt để vòng đời. Trả trạng thái chương trình về `0` (Success).

---

## 🔥 Điểm Nổi Bật Về Kỹ Thuật Trong Dự Án Này

* **Không Memory Leak (Rò rỉ bộ nhớ):** Quy trình tạo `new` của `FlightManager` và `Flight` đều được kiểm soát và thu dọn `delete` tận tụy tại Destructor của chúng, ngăn phần mềm chiếm dụng bộ nhớ RAM của hệ thống.
* **Quyển sổ thông tin file song song:** File `seats_input.txt` đóng vai trò là Mapping vật lý, trong khi file `booking_output.txt` ghi log hành vi theo thời gian thực (Append mode). Mọi giao dịch đặt xong đều đẩy ngay vào ổ cứng để ngăn mất mát dữ liệu nếu hệ thống tắt ngang.
