# ROADMAP DỰ ÁN CUỐI HỌC PHẦN OOP C++

## Hệ thống quản lý bãi đỗ xe thông minh — Console

**Phiên bản:** 1.1 · **Ngày lập:** 06/11/2026  
**Mục tiêu:** Xây dựng một ứng dụng C++ có thể chạy, kiểm thử và bảo vệ trước giảng viên, trong đó **thiết kế hướng đối tượng là trọng tâm**.

**Cập nhật phiên bản 1.1:** Bổ sung lưu trữ lâu dài bằng file CSV/TXT theo yêu cầu. Ứng dụng tự nạp khi mở, tự lưu sau thay đổi và có menu nhập/xuất thủ công. RAM dùng để xử lý trong phiên chạy; dữ liệu được giữ trong file qua các lần mở chương trình.

> **Định nghĩa “thông minh” trong phạm vi dự án:** Hệ thống tự động phân bổ ô đỗ theo loại xe và khoảng cách, cảnh báo gần đầy, hỗ trợ các chính sách tính phí có thể thay đổi. **Không** tuyên bố sử dụng AI, camera hay cảm biến.

---

## 1. Ràng buộc bắt buộc

| Tiêu chí | Quyết định |
|---|---|
| Ngôn ngữ | **C++11 trở lên**, ưu tiên tương thích MinGW `g++` 6.3 |
| Giao diện thao tác | **Console**, menu số và nhập dữ liệu bằng bàn phím |
| Cấu trúc nguồn | **Chỉ `.h` và `.cpp`**; tách khai báo/định nghĩa |
| Biên dịch | Dùng một lệnh `g++` trực tiếp trong Terminal |
| Không sử dụng | CMake, qmake/`.pro`, Qt, GUI framework, database |
| Trọng tâm đánh giá | Đóng gói, kế thừa, đa hình, trừu tượng, quan hệ lớp, `static`, nạp chồng toán tử |
| Lưu trữ | **File CSV**, hỗ trợ đuôi `.txt` cùng định dạng; tự lưu/nạp và nhập/xuất từ menu |

**Nguyên tắc:** Không thêm tính năng chỉ để dự án trông lớn hơn. Mỗi lớp phải có trách nhiệm thực sự; mọi kỹ thuật OOP phải giải thích được vì sao cần dùng.

## 2. Phạm vi chức năng

### 2.1. Phải có (MVP)

1. Khởi tạo một bãi đỗ với các ô cho **xe máy** và **ô tô**.
2. Hiển thị ô trống/ô đã có xe, mã ô và khoảng cách đến cổng.
3. Nhận xe vào: nhập loại xe, biển số, thời điểm vào.
4. **Tự động chọn ô còn trống đúng loại và gần cổng nhất**.
5. Tạo vé có mã duy nhất bằng bộ đếm `static`.
6. Tìm vé/xe đang gửi theo mã vé hoặc biển số.
7. Trả xe: kiểm tra mã vé, tính thời gian và số tiền, giải phóng ô.
8. Thống kê số ô còn trống, tỷ lệ sử dụng, doanh thu từ các lượt đã kết thúc.
9. Báo lỗi hợp lý khi bãi hết chỗ, biển số đang gửi bị trùng, mã vé không tồn tại hoặc thời gian không hợp lệ.
10. Lưu và khôi phục toàn bộ ô đỗ, vé đang gửi, lịch sử, doanh thu, chính sách phí và bộ đếm mã vé bằng file; từ chối file không hợp lệ mà không làm thay đổi trạng thái hiện tại.

### 2.2. Nên có sau khi hoàn tất MVP

- Hai chính sách thu phí: **theo giờ** và **theo lượt**, chuyển đổi được từ menu.
- Cảnh báo tỷ lệ lấp đầy **từ 80%**.
- Thống kê theo loại xe; xem lịch sử vé đã trả.
- Nạp chồng `operator<<` để in một đối tượng ra `cout` (nếu muốn minh họa `friend`).

### 2.3. Không làm ở phiên bản cuối học phần

Nhận diện biển số, cảm biến, IoT, tài khoản đăng nhập, phân quyền, thanh toán thật, cơ sở dữ liệu, ứng dụng web, GUI, ML hoặc tìm đường phức tạp. Đây là những phần mở rộng, không cần thiết để chứng minh năng lực OOP.

---

## 3. Quy ước nghiệp vụ cần chốt trước khi code

**Loại phương tiện:** `XeMay` và `OTo`. Xe máy chỉ vào ô xe máy, ô tô chỉ vào ô ô tô.

**Dữ liệu bãi ban đầu:** 6 ô xe máy và 4 ô ô tô; mỗi ô có một khoảng cách tới cổng (số nguyên không âm). Dữ liệu được khởi tạo trực tiếp trong code.

**Chọn ô:** Lọc các ô **đang trống + đúng loại xe**, sau đó chọn ô có **khoảng cách nhỏ nhất**. Nếu hai ô cách cổng bằng nhau, lấy ô có mã nhỏ hơn. Không cần thuật toán tối ưu phức tạp.

**Thời gian để dễ demo:** Nhập số phút kể từ một mốc mô phỏng chung. Ví dụ `480 = 08:00`, `555 = 09:15`, thời gian gửi `75 phút`. Nếu qua ngày thì tiếp tục đếm (ví dụ `1500`), không reset về `0`. Điều kiện: `phút ra >= phút vào`.

**Tính phí minh họa:**

| Chính sách | Xe máy | Ô tô |
|---|---:|---:|
| Theo giờ | 3.000 đ/giờ | 10.000 đ/giờ |
| Theo lượt | 8.000 đ/lượt | 25.000 đ/lượt |

Với tính theo giờ: `sốGiờ = max(1, (sốPhútGửi + 59) / 60)` (làm tròn lên, tối thiểu một giờ). Đây là **mức phí giả định để thử nghiệm**, không phải bảng giá thực tế.

**Quy tắc thay chính sách:** Trong bản cơ bản, chính sách đang chọn khi **trả xe** được dùng để tính phí. Nếu muốn bảo toàn giá lúc vào bãi, có thể bổ sung ở giai đoạn nâng cao.

**Quy tắc dữ liệu:** Không cho hai vé **đang hoạt động** có cùng biển số; sau khi xe ra, biển số có thể vào lại. Doanh thu chỉ cộng **một lần** khi đóng vé thành công.

**Quy tắc lưu trữ:** File mặc định `parking_data.csv` nằm cạnh chương trình trên Windows. Tự nạp lúc khởi động và tự lưu sau khi nhận xe, trả xe, đổi chính sách hoặc nhập dữ liệu. Có thể chọn file chính khác bằng tham số dòng lệnh. Menu nhập file **thay thế toàn bộ dữ liệu**, không gộp. Mã vé đã lưu được giữ nguyên; bộ đếm không giảm khi nhập bản sao cũ. Doanh thu khôi phục bằng tổng phí lịch sử, không tính lại theo chính sách hiện tại. File được kiểm tra toàn bộ trước khi áp dụng; ghi qua file tạm rồi thay thế để giữ bản cũ nếu ghi thất bại. Chi tiết định dạng và file mẫu có trong `README.md` và `du_lieu_mau.csv`.

---

## 4. Kiến trúc lớp hướng đối tượng

### 4.1. Các lớp chính

| Lớp | Trách nhiệm | Kiến thức OOP |
|---|---|---|
| `Vehicle` | Lớp cơ sở trừu tượng, lưu biển số, định nghĩa giao diện lấy loại xe | Trừu tượng, đóng gói |
| `Motorbike`, `Car` | Hai lớp con xác định loại phương tiện | Kế thừa, `override` |
| `ParkingSlot` | Quản lý trạng thái và mức ưu tiên của một ô đỗ | Đóng gói, nạp chồng `<` |
| `ParkingTicket` | Quản lý một lượt gửi xe: mã, xe, ô, thời gian, phí | Constructor, `static`, quan hệ lớp |
| `FeePolicy` | Giao diện chung để tính phí | Lớp trừu tượng, hàm `virtual` |
| `HourlyFeePolicy`, `FlatFeePolicy` | Hai cách tính phí khác nhau | Đa hình |
| `ParkingManager` | Điều phối nhận xe, trả xe, chọn ô và thống kê | Phối hợp nhiều đối tượng |
| `ParkingFileStorage` | Đọc/ghi CSV, kiểm tra dữ liệu và khôi phục trạng thái | Tách trách nhiệm, `friend` có giới hạn |
| `main` | Menu Console, nhận dữ liệu, gọi lớp quản lý, hiển thị kết quả | Tách giao diện khỏi nghiệp vụ |

### 4.2. Sơ đồ quan hệ đơn giản

```text
             Vehicle (abstract)
             /               \
         Motorbike            Car
             \               /
              \             /
               ParkingTicket
                    |
                    | được quản lý bởi
                    v
              ParkingManager ---- quản lý ----> ParkingSlot
                    |
                    | sử dụng
                    v
             FeePolicy (abstract)
             /                 \
    HourlyFeePolicy        FlatFeePolicy

main.cpp (Console) ---- gọi ----> ParkingManager
```

### 4.3. Phân công thuộc tính, phương thức

**`Vehicle`, `Car`, `Motorbike`:**

- `Vehicle` lưu `std::string BienSo` và cung cấp `GetBienSo() const`.
- Có `virtual VehicleType GetLoai() const = 0;`.
- Có `virtual ~Vehicle()` để hủy đúng đối tượng qua con trỏ lớp cơ sở.
- Dùng `enum class VehicleType { XeMay, OTo };` thay vì chuỗi tự do.
- `Car` và `Motorbike` ghi đè `GetLoai()`.

**`ParkingSlot`:**

- Thuộc tính riêng: `MaO`, `LoaiO`, `KhoangCach`, `DangCoXe`.
- Phương thức đọc thông tin, `ChiếmÔ()`/`GiaiPhongO()` (khi viết code nên đặt tên **không dấu**, ví dụ `ChiemO()`).
- Nạp chồng `bool operator<(const ParkingSlot& other) const;` để xếp ưu tiên theo khoảng cách, hòa thì xét mã ô.
- Không cho mã nguồn bên ngoài tùy tiện gán `DangCoXe = false`.

**`ParkingTicket`:**

- Thuộc tính: `static int SoVeDaTao`, `MaVe`, `MaO`, `PhutVao`, `PhutRa`, `Phi`, `DaKetThuc`.
- Lưu phương tiện bằng `std::unique_ptr<Vehicle>` để có thể chứa cả ô tô lẫn xe máy.
- Constructor tăng `SoVeDaTao` và tạo `MaVe` riêng cho từng vé.
- Hàm đóng vé kiểm tra thời gian và ngăn tính phí hai lần.

**`FeePolicy`, `HourlyFeePolicy`, `FlatFeePolicy`:**

- Giao diện chung: `virtual long long TinhPhi(VehicleType loai, int soPhut) const = 0;`.
- Lớp con thực thi công thức thu phí tương ứng.
- Có destructor `virtual`.
- `ParkingManager` giữ `std::unique_ptr<FeePolicy>` để thay đối tượng chính sách lúc chạy.

**`ParkingManager`:**

- Quản lý `std::vector<ParkingSlot>` chứa các ô.
- Quản lý vé đang gửi và lịch sử bằng `std::vector<std::unique_ptr<ParkingTicket>>`.
- Chứa một chính sách phí đang hoạt động.
- Các nhóm phương thức: `NhanXe(...)`, `TraXe(...)`, `TimVe(...)`, `LayDanhSachO(...)`, `ThongKe(...)`, `DoiChinhSachPhi(...)`.
- Có `XuatFile(...)` và `NhapFile(...)` gọi lớp lưu trữ; `ParkingFileStorage` kiểm tra và tạo dữ liệu tạm trước khi thay thế trạng thái quản lý.
- Mọi quy tắc nghiệp vụ (ô hợp lệ, biển số trùng, giờ ra hợp lệ) nằm tại đây hoặc trong lớp dữ liệu phù hợp, **không nằm rải rác ở `main.cpp`**.

**Lưu ý quan trọng:** `std::unique_ptr` là kiến thức C++ bổ sung cho quản lý bộ nhớ an toàn. Nếu học phần chưa dạy, có thể làm phiên bản dễ hơn trước, nhưng cần tránh dùng con trỏ thô với `new/delete` thiếu kiểm soát.

### 4.4. Ánh xạ tới những nội dung OOP đã học

| Nội dung học phần | Vị trí minh họa trong dự án |
|---|---|
| `class`, `private`, `public` | `ParkingSlot`, `ParkingTicket` |
| Constructor / destructor | Tất cả lớp, đặc biệt `Vehicle` |
| `static` | `ParkingTicket::SoVeDaTao` |
| Kế thừa | `Car`, `Motorbike` kế thừa `Vehicle` |
| Hàm ảo, lớp trừu tượng | `Vehicle::GetLoai()`, `FeePolicy::TinhPhi()` |
| Đa hình | Chuyển đổi `HourlyFeePolicy` / `FlatFeePolicy` |
| Nạp chồng toán tử | `ParkingSlot::operator<` |
| `friend` (tùy chọn) | `operator<<` để in thông tin ô hoặc vé |
| `const`, tham chiếu | Getter, hàm so sánh, truyền `const T&` |
| Thành phần và phối hợp lớp | `ParkingManager` sở hữu ô, vé, chính sách |
| Xử lý ngoại lệ | Biển số trùng, hết chỗ, thời gian sai |

> Không nhất thiết phải viết **cả Member và Friend cho cùng một toán tử** như trong bài tập riêng `Fraction`, trừ khi giảng viên yêu cầu. Trong dự án, chọn cách có ý nghĩa thiết kế và giải thích được lý do.

---

## 5. Cấu trúc file — chỉ `.h` và `.cpp`

```text
ParkingOOP/
├── Vehicle.h
├── Vehicle.cpp
├── ParkingSlot.h
├── ParkingSlot.cpp
├── ParkingTicket.h
├── ParkingTicket.cpp
├── FeePolicy.h
├── FeePolicy.cpp
├── ParkingManager.h
├── ParkingManager.cpp
├── ParkingFileStorage.h
├── ParkingFileStorage.cpp
└── main.cpp
```

Tổng cộng **13 file mã nguồn**. Để không phân mảnh quá mức, `Vehicle.h/.cpp` chứa cả lớp cơ sở và hai lớp phương tiện; `FeePolicy.h/.cpp` chứa lớp cơ sở và hai chính sách phí. `ParkingFileStorage.h/.cpp` bổ sung trách nhiệm đọc/ghi file, không đặt xử lý CSV trong menu.

Quy tắc viết code:

- `.h` dùng `#ifndef ... #define ... #endif` hoặc `#pragma once`.
- `.h` chủ yếu khai báo lớp và chữ ký hàm; `.cpp` định nghĩa các hàm.
- Không `#include` file `.cpp` vào file khác.
- `main.cpp` chỉ đảm nhiệm menu và giao tiếp với người dùng.
- Không tạo `CMakeLists.txt`, `.pro` hay file cấu hình build.
- Nếu thêm file trong quá trình phát triển, chỉ thêm `.h/.cpp` khi có lớp mới thực sự cần thiết.

---

## 6. Roadmap thực hiện — 5 giai đoạn trong 4–5 tuần

### Giai đoạn 0 — Chốt thiết kế (1 buổi)

- [ ] Xác nhận loại xe, số lượng ô và khoảng cách từng ô.
- [ ] Xác nhận quy tắc nhận xe, trả xe, chọn ô, tính phí.
- [ ] Vẽ sơ đồ lớp và giải thích trách nhiệm mỗi lớp bằng lời.
- [ ] Xác định rõ phạm vi MVP, tránh thêm GUI/AI/IoT.

**Đạt khi:** Có thể mô tả toàn bộ luồng xe vào → cấp vé → giữ ô → trả xe → tính phí → giải phóng ô **mà chưa cần code**.

### Giai đoạn 1 — Các lớp nền tảng (tuần 1)

- [ ] Tạo `Vehicle.h/.cpp`, `Car`, `Motorbike`; kiểm tra hàm ảo `GetLoai()`.
- [ ] Tạo `ParkingSlot.h/.cpp`; test ô trống/đầy và `operator<`.
- [ ] Tạo `ParkingTicket.h/.cpp`; cấp mã vé tự tăng bằng `static`.
- [ ] Kiểm tra dữ liệu đầu vào cơ bản: biển số rỗng, số phút âm, khoảng cách âm.
- [ ] Viết một `main.cpp` tạm thời để thử các lớp riêng lẻ.

**Đạt khi:** Tạo được xe, ô, vé; chứng minh `static`, kế thừa và nạp chồng toán tử hoạt động.

### Giai đoạn 2 — Nghiệp vụ bãi xe (tuần 2)

- [ ] Xây dựng `ParkingManager` với 6 ô xe máy, 4 ô ô tô.
- [ ] Viết hàm lọc ô trống đúng loại, chọn gần cổng nhất.
- [ ] Xây dựng `NhanXe`: kiểm tra biển số, phân ô, cấp vé.
- [ ] Xây dựng tìm kiếm vé theo mã và biển số.
- [ ] Xây dựng `TraXe`: xác nhận vé và giờ ra, gỡ trạng thái chiếm ô, chuyển vé sang lịch sử.
- [ ] Không để thao tác thất bại làm thay đổi dữ liệu nửa chừng.

**Đạt khi:** Hai xe có thể vào, hệ thống chọn ô hợp lệ; trả một xe thì đúng ô đó trở lại trống.

### Giai đoạn 3 — Đa hình và phần “thông minh” (tuần 3)

- [ ] Tạo `FeePolicy` trừu tượng.
- [ ] Viết `HourlyFeePolicy` và `FlatFeePolicy`.
- [ ] `ParkingManager` tính phí qua giao diện chung `TinhPhi()` (không cần `if/else` theo loại chính sách).
- [ ] Cho phép đổi chính sách từ menu.
- [ ] Thống kê tổng ô, ô trống, tỷ lệ lấp đầy, doanh thu.
- [ ] Cảnh báo khi tỷ lệ sử dụng đạt 80%.

**Đạt khi:** Có thể thay cách tính phí trong lúc chạy mà không sửa quy trình `TraXe`. Đây là điểm demo đa hình quan trọng.

### Giai đoạn 4 — Hoàn thiện Console và kiểm thử (tuần 4)

- [ ] Tạo menu lặp cho đến khi người dùng chọn thoát.
- [ ] Tách nhập/xuất Console khỏi các lớp nghiệp vụ.
- [ ] Bắt ngoại lệ ở `main.cpp`; thông báo lỗi dễ hiểu và quay lại menu.
- [ ] Xử lý `cin.fail()` để không mắc vòng lặp khi nhập chữ thay vì số.
- [ ] Hoàn thiện `ParkingFileStorage`, menu nhập/xuất, tự lưu/nạp, giữ nguyên dữ liệu nếu nạp file lỗi và giữ file cũ nếu ghi thất bại.
- [ ] Kiểm tra các ca ở Mục 8.
- [ ] Build lại từ đầu bằng **một lệnh `g++`**, không phụ thuộc IDE.

**Đạt khi:** Chương trình demo liên tục, không bị crash ở các lỗi nhập liệu thường gặp.

### Giai đoạn 5 — Chuẩn bị bảo vệ (3–5 ngày cuối)

- [ ] Hoàn thiện sơ đồ lớp UML.
- [ ] Xác định 5–7 đoạn code minh họa OOP trọng tâm.
- [ ] Chuẩn bị kịch bản demo 3–5 phút.
- [ ] Tập giải thích cách chạy từ `.h/.cpp` đến `.exe`.
- [ ] Chuẩn bị câu trả lời về `virtual`, `override`, `static`, `const`, `friend`, `operator<`, quan hệ lớp.
- [ ] Chạy lại tình huống bình thường và tình huống lỗi trước khi nộp.

**Đạt khi:** Có thể giải thích được **tại sao thiết kế như vậy**, không chỉ trình bày những tính năng đã code.

---

## 7. Menu Console dự kiến

```text
=============== QUAN LY BAI DO XE ===============
1. Xem danh sach o do
2. Nhan xe vao bai
3. Tra xe va tinh phi
4. Tra cuu ve / bien so
5. Danh sach xe dang gui
6. Thong ke va doanh thu
7. Thay doi chinh sach tinh phi
8. Lich su ve da tra
9. Xuat file CSV / TXT
10. Nhap file CSV / TXT
0. Thoat
================================================
Nhap lua chon: _
```

**Kịch bản demo mẫu:**

1. Xem bãi đang có 6 ô xe máy và 4 ô ô tô trống.
2. Nhận xe máy `29A-12345`, thời điểm vào `480` (08:00).
3. Hệ thống chọn ô xe máy **gần cổng nhất** và tự cấp vé `1`.
4. Tra cứu vé `1`, xác nhận đúng biển số và ô đỗ.
5. Trả xe ở phút `555` (09:15): gửi `75` phút, tính thành `2` giờ.
6. Theo chính sách giờ: `2 × 3.000 = 6.000 VND`.
7. Xem ô đã giải phóng và doanh thu tăng `6.000 VND`.
8. Thử nhập lại biển số khi xe vẫn đang gửi hoặc cho xe vào khi hết ô để trình diễn xử lý lỗi.

---

## 8. Danh sách ca kiểm thử

| Mã | Tình huống | Kết quả cần đạt |
|---|---|---|
| T01 | Tạo `Car`, `Motorbike` | Nhận đúng loại xe qua `Vehicle` |
| T02 | Cho xe vào lúc còn chỗ | Chọn ô đúng loại và gần cổng |
| T03 | Hai ô có cùng khoảng cách | Ưu tiên mã ô nhỏ hơn |
| T04 | Biển số đã có xe đang gửi | Từ chối, không tạo vé mới |
| T05 | Hết ô xe máy, còn ô ô tô | Không lấy nhầm ô ô tô |
| T06 | Xe gửi 75 phút | Phí giờ tính theo 2 giờ |
| T07 | Giờ ra nhỏ hơn giờ vào | Báo lỗi, không giải phóng ô |
| T08 | Mã vé không tồn tại | Báo lỗi, không thay đổi dữ liệu |
| T09 | Trả lại vé đã đóng | Từ chối, không cộng doanh thu lần 2 |
| T10 | Đổi chính sách tính phí | Lấy công thức mới đúng quy ước |
| T11 | Tỷ lệ lấp đầy từ 80% | Hiện cảnh báo |
| T12 | Tạo nhiều vé liên tiếp | Mỗi vé có mã khác nhau |
| T13 | Nhập chữ thay cho số phút | Báo nhập sai, cho nhập lại |
| T14 | Trả hết xe | Tỷ lệ sử dụng về 0%, doanh thu được giữ |
| T15 | Biển số rỗng / phút âm | Từ chối dữ liệu không hợp lệ |

**Nguyên tắc test:** Khi một thao tác thất bại, trạng thái hệ thống phải giữ nguyên: không “mất” ô, không có vé ảo, không tăng doanh thu.

**Ca kiểm thử lưu trữ bổ sung:**

- **T16:** Xuất rồi nhập CSV/TXT: giữ đúng mã vé, ô có xe, lịch sử, doanh thu và chính sách phí.
- **T17:** Đóng và mở lại chương trình: khôi phục dữ liệu, cấp mã vé tiếp theo.
- **T18:** Nhập file lỗi, thiếu phần kết thúc, trùng mã vé/biển số/ô, sai loại xe hoặc sai thời gian: từ chối, không đổi dữ liệu và bộ đếm.
- **T19:** File đích không ghi được: giữ bản cũ, báo dữ liệu chưa lưu và cho phép xuất bản sao khác.
- **T20:** CSV có dấu phẩy/nháy kép, BOM và CRLF: đọc/ghi đúng dữ liệu.
- **T21:** Nhập bản sao cũ: thay thế dữ liệu nhưng không làm giảm bộ đếm vé đã cấp.

---

## 9. Biên dịch trực tiếp bằng g++ (Windows PowerShell)

Đứng tại thư mục `ParkingOOP` rồi chạy:

```powershell
g++ -std=c++11 -Wall -Wextra main.cpp Vehicle.cpp ParkingSlot.cpp ParkingTicket.cpp FeePolicy.cpp ParkingManager.cpp ParkingFileStorage.cpp -o ParkingOOP.exe
```

Nếu không có lỗi:

```powershell
.\ParkingOOP.exe
```

Hoặc dùng một lệnh chỉ chạy khi build thành công:

```powershell
g++ -std=c++11 -Wall -Wextra main.cpp Vehicle.cpp ParkingSlot.cpp ParkingTicket.cpp FeePolicy.cpp ParkingManager.cpp ParkingFileStorage.cpp -o ParkingOOP.exe; if ($LASTEXITCODE -eq 0) { .\ParkingOOP.exe }
```

Không dùng `g++ *.cpp` nếu đặt chung thư mục với các bài thực hành `Fraction`, `Complex`, `Student` có nhiều hàm `main()`.

---

## 10. Các câu hỏi có thể gặp khi bảo vệ

1. **Tại sao `Vehicle` là lớp trừu tượng?** Vì hệ thống làm việc với phương tiện cụ thể là xe máy hoặc ô tô, nhưng cần giao diện thống nhất để thao tác.
2. **`virtual` và `override` có tác dụng gì?** Gọi đúng phương thức của lớp con thông qua kiểu lớp cơ sở.
3. **Tại sao cần destructor `virtual`?** Vì có thể hủy đối tượng lớp con qua con trỏ lớp cha.
4. **Vì sao bộ đếm vé là `static`, còn mã vé không phải?** Bộ đếm được chia sẻ cho toàn lớp; mã vé là dữ liệu riêng từng đối tượng.
5. **Nạp chồng `ParkingSlot::operator<` để làm gì?** So sánh mức ưu tiên ô đỗ theo khoảng cách và mã ô.
6. **Vì sao tách `FeePolicy` thành nhiều lớp?** Để minh họa đa hình và thay đổi cách tính phí mà không sửa logic quản lý xe.
7. **Tại sao truyền `const T&`?** Tránh sao chép không cần thiết và không sửa đối tượng thông qua tham chiếu.
8. **`std::unique_ptr` có ích gì?** Rõ quyền sở hữu, tự giải phóng bộ nhớ, tránh lỗi `delete`.
9. **Tại sao `main.cpp` không tự tìm ô và tính tiền?** Vì giao diện nên tách khỏi nghiệp vụ; dễ bảo trì, kiểm thử và thay đổi.
10. **Yếu tố “thông minh” ở đâu?** Ở phân bổ ô tự động theo quy tắc và cảnh báo đầy bãi; không phải AI.

---

## 11. Mốc tự đánh giá trước khi nộp

- [ ] Chỉ cần các file `.h/.cpp` và một lệnh `g++` để build.
- [ ] Các thuộc tính trọng yếu được đóng gói trong lớp.
- [ ] Có **kế thừa thực sự** giữa `Vehicle` và các loại xe.
- [ ] Có **đa hình chạy thực tế** qua `FeePolicy`.
- [ ] Có **`static` và nạp chồng toán tử** đúng ngữ nghĩa.
- [ ] Có kiểm tra nhập liệu và các trường hợp bất hợp lệ.
- [ ] Không trùng biển số đang gửi, không đỗ hai xe cùng một ô.
- [ ] Không tính tiền hoặc cộng doanh thu hai lần cho một vé.
- [ ] Đóng/mở lại không mất dữ liệu; nhập/xuất file đúng và xử lý file lỗi an toàn.
- [ ] Có thể trình diễn tất cả chức năng cốt lõi trong 3–5 phút.
- [ ] Từng thành viên nhóm (nếu làm nhóm) giải thích được phần code của mình.

---

## 12. Việc cần làm ngay — không viết toàn bộ dự án một lần

**Bước tiếp theo là Giai đoạn 1**, theo thứ tự:

1. Tạo thư mục `ParkingOOP` và 13 file đã liệt kê; cặp file lưu trữ triển khai sau khi nghiệp vụ nền đã hoạt động.
2. Làm `Vehicle.h/.cpp` trước: lớp trừu tượng và hai lớp kế thừa.
3. Kiểm thử riêng hai kiểu xe trong `main.cpp` tạm.
4. Làm `ParkingSlot.h/.cpp` và thử so sánh ô bằng `operator<`.
5. Làm `ParkingTicket.h/.cpp`, thử bộ đếm `static` sinh mã vé.
6. Khi ba phần này chạy đúng mới bắt đầu `ParkingManager` và `FeePolicy`.

**Tiêu chí xuyên suốt:** Hoàn thành đến đâu, build và tự giải thích được đến đó. Ưu tiên tính đúng đắn và kiến thức OOP hơn số lượng tính năng.
