# Tài Liệu Thiết Kế UML & Luồng Dữ Liệu (Dataflow) — Dự Án Quản Lý Bãi Đỗ Xe OOP C++

Tài liệu này mô tả chi tiết kiến trúc hướng đối tượng (**UML**) và luồng di chuyển, biến đổi dữ liệu (**Dataflow Diagram - DFD**) của hệ thống **Quản lý bãi đỗ xe thông minh trên Console** (`ParkingOOP`).

---

## Phần 1: Hệ Thống Sơ Đồ UML (Unified Modeling Language)

### 1.1. Sơ Đồ Lớp Chi Tiết (UML Class Diagram)

Sơ đồ dưới đây biểu diễn toàn bộ các lớp, cấu trúc (`struct`), kiểu liệt kê (`enum class`), thuộc tính, phương thức và mối quan hệ thực tế trong mã nguồn:

```mermaid
classDiagram
    direction TB

    class VehicleType {
        <<enumeration>>
        XeMay
        OTo
    }

    class Vehicle {
        <<abstract>>
        -string BienSo
        +Vehicle(bienSo : const string&)
        +~Vehicle()*
        +GetBienSo() const string&
        +GetLoai() const VehicleType*
        +ChuanHoaBienSo(bienSo : const string&) string$
    }

    class Motorbike {
        +Motorbike(bienSo : const string&)
        +GetLoai() const VehicleType
    }

    class Car {
        +Car(bienSo : const string&)
        +GetLoai() const VehicleType
    }

    class ParkingSlot {
        -int MaO
        -VehicleType LoaiO
        -int KhoangCach
        -bool DangCoXe
        +ParkingSlot(maO : int, loaiO : VehicleType, khoangCach : int)
        +GetMaO() const int
        +GetLoaiO() const VehicleType
        +GetKhoangCach() const int
        +CoXe() const bool
        +ChiemO() void
        +GiaiPhongO() void
        +operator~(other : const ParkingSlot&) const bool
        +operator~~(out : ostream&, o : const ParkingSlot&) ostream&
    }

    class ParkingTicket {
        -int SoVeDaTao$
        -unique_ptr~Vehicle~ PhuongTien
        -int MaVe
        -int MaO
        -int PhutVao
        -int PhutRa
        -long long Phi
        -bool KetThuc
        -ParkingTicket(xe, maO, phutVao, maVe, phutRa, phi, daKetThuc)
        +ParkingTicket(xe : unique_ptr~Vehicle~, maO : int, phutVao : int)
        +GetSoVeDaTao() int$
        +GetMaVe() const int
        +GetMaO() const int
        +GetXe() const Vehicle&
        +GetPhutVao() const int
        +GetPhutRa() const int
        +GetPhi() const long long
        +DaKetThuc() const bool
        +DongVe(phutRa : int, phi : long long) void
        +operator~~(out : ostream&, ve : const ParkingTicket&) ostream&
    }

    class FeePolicy {
        <<abstract>>
        +~FeePolicy()*
        +TinhPhi(loai : VehicleType, soPhut : int) const long long*
        +GetTen() const char**
    }

    class HourlyFeePolicy {
        +TinhPhi(loai : VehicleType, soPhut : int) const long long
        +GetTen() const char*
    }

    class FlatFeePolicy {
        +TinhPhi(loai : VehicleType, soPhut : int) const long long
        +GetTen() const char*
    }

    class ThongKeLoaiXe {
        <<struct>>
        +int TongO
        +int OTrong
        +int DangGui
        +double TyLeSuDung
    }

    class ThongKeBaiXe {
        <<struct>>
        +int TongO
        +int OTrong
        +int DangGui
        +int SoLuotDaTra
        +double TyLeSuDung
        +long long DoanhThu
        +ThongKeLoaiXe XeMay
        +ThongKeLoaiXe OTo
        +bool CanhBaoGanDay
    }

    class ParkingManager {
        -vector~ParkingSlot~ DanhSachO
        -vector~unique_ptr~ParkingTicket~~ VeDangGui
        -vector~unique_ptr~ParkingTicket~~ LichSuVe
        -unique_ptr~FeePolicy~ ChinhSachPhi
        -long long DoanhThu
        -ChonO(loai : VehicleType) ParkingSlot*
        -TimOTheoMa(maO : int) ParkingSlot*
        +ParkingManager()
        +NhanXe(loai : VehicleType, bienSo : const string&, phutVao : int) const ParkingTicket&
        +TraXe(maVe : int, phutRa : int) const ParkingTicket&
        +TimVe(maVe : int) const ParkingTicket*
        +TimXeDangGui(bienSo : const string&) const ParkingTicket*
        +LayDanhSachO() const vector~ParkingSlot~&
        +LayVeDangGui() const vector~const ParkingTicket*~
        +LayLichSuVe() const vector~const ParkingTicket*~
        +DoiChinhSachPhi(chinhSach : unique_ptr~FeePolicy~) void
        +LayChinhSachPhi() const FeePolicy&
        +ThongKe() const ThongKeBaiXe
        +XuatFile(duongDan : const string&) const void
        +NhapFile(duongDan : const string&) void
    }

    class ParkingFileStorage {
        +Luu(quanLy : const ParkingManager&, duongDan : const string&) void$
        +Nap(quanLy : ParkingManager&, duongDan : const string&) void$
    }

    Vehicle <|-- Motorbike : Kế thừa (is-a)
    Vehicle <|-- Car : Kế thừa (is-a)
    FeePolicy <|-- HourlyFeePolicy : Kế thừa (is-a)
    FeePolicy <|-- FlatFeePolicy : Kế thừa (is-a)

    ParkingTicket "1" *-- "1" Vehicle : Sở hữu (unique_ptr)
    ParkingManager "1" *-- "10" ParkingSlot : Sở hữu (vector)
    ParkingManager "1" *-- "0..*" ParkingTicket : Sở hữu VeDangGui & LichSuVe
    ParkingManager "1" *-- "1" FeePolicy : Sở hữu ChinhSachPhi
    ThongKeBaiXe "1" *-- "2" ThongKeLoaiXe : XeMay & OTo

    ParkingSlot --> VehicleType : Phân loại ô
    Vehicle --> VehicleType : Phân loại xe
    ParkingTicket ..> ParkingSlot : Tham chiếu qua MaO
    ParkingManager ..> ThongKeBaiXe : Khởi tạo & trả về
    ParkingManager <..> ParkingFileStorage : friend class / Đọc & Ghi CSV
    ParkingTicket <.. ParkingFileStorage : friend class (khôi phục vé)
```

> **Ghi chú ký hiệu:** Trong Mermaid Class Diagram ở trên, `operator~` đại diện cho `ParkingSlot::operator<` và `operator~~` đại diện cho toán tử luồng xuất `friend operator<<` tại `ParkingSlot.h` và `ParkingTicket.h`.

#### Bảng Phân Tích Quan Hệ Giữa Các Lớp

| Quan hệ OOP | Lớp nguồn -> Lớp đích | Biểu diễn trong Code | Ý nghĩa thiết kế |
|---|---|---|---|
| **Inheritance (Kế thừa)** | `Motorbike`, `Car` -> `Vehicle` | `class Motorbike : public Vehicle` | Tái sử dụng thuộc tính `BienSo` và chuẩn hóa biển số; đa hình qua hàm thuần ảo `GetLoai()`. |
| **Inheritance (Kế thừa)** | `HourlyFeePolicy`, `FlatFeePolicy` -> `FeePolicy` | `class HourlyFeePolicy : public FeePolicy` | Thực thi mẫu thiết kế **Strategy Pattern**: đóng gói thuật toán tính phí theo giờ và theo lượt. |
| **Composition (Hợp thành)** | `ParkingTicket` *-- `Vehicle` | `std::unique_ptr<Vehicle> PhuongTien` | Mỗi vé gắn chặt với duy nhất một phương tiện; khi vé bị hủy, phương tiện tự động được giải phóng (RAII). |
| **Composition (Hợp thành)** | `ParkingManager` *-- `ParkingSlot`, `ParkingTicket`, `FeePolicy` | `std::vector<ParkingSlot>`, `std::vector<std::unique_ptr<ParkingTicket>>`, `std::unique_ptr<FeePolicy>` | `ParkingManager` làm chủ toàn bộ vòng đời của các ô đỗ, vé đang gửi, lịch sử vé và chính sách phí hiện hành. |
| **Friendship (`friend`)** | `ParkingFileStorage` -> `ParkingTicket`, `ParkingManager` | `friend class ParkingFileStorage;` | Cho phép lớp lưu trữ gọi constructor `private` của `ParkingTicket` để khôi phục mã vé cũ và hoán đổi (`swap`) trạng thái `ParkingManager` mà không cần mở hàm `Setter` công khai phá vỡ tính đóng gói. |

---

### 1.2. Sơ Đồ Tuần Tự (UML Sequence Diagrams)

#### Kịch bản 1: Quy trình Nhận Xe Vào Bãi (`NhanXe`)

```mermaid
sequenceDiagram
    autonumber
    actor User as Người dùng (Console)
    participant Main as main.cpp
    participant PM as ParkingManager
    participant Veh as Vehicle / Motorbike / Car
    participant Slot as ParkingSlot
    participant Ticket as ParkingTicket
    participant Storage as ParkingFileStorage

    User->>Main: Chọn Menu 2 (Nhập loại xe, biển số, phút vào)
    Main->>PM: NhanXe(loai, bienSo, phutVao)
    PM->>Veh: ChuanHoaBienSo(bienSo)
    Veh-->>PM: bienSoChuan (viết hoa, bỏ khoảng trắng)
    PM->>PM: TimXeDangGui(bienSoChuan)
    alt Biển số đang gửi trong bãi
        PM-->>Main: throw invalid_argument("Bien so nay da co xe dang gui...")
        Main-->>User: Hiển thị thông báo lỗi
    else Biển số hợp lệ
        PM->>PM: ChonO(loai)
        loop Duyệt DanhSachO[0..9]
            PM->>Slot: !CoXe() && GetLoaiO() == loai && o < *oTotNhat
        end
        alt Hết ô trống cùng loại
            PM-->>Main: throw runtime_error("Da het o trong cho loai xe nay.")
            Main-->>User: Hiển thị thông báo lỗi
        else Tìm thấy ô tốt nhất (oTotNhat)
            PM->>Veh: new Motorbike(bienSoChuan) hoặc new Car(bienSoChuan)
            PM->>Ticket: new ParkingTicket(move(xe), o->GetMaO(), phutVao)
            Ticket->>Ticket: MaVe = ++SoVeDaTao
            PM->>Slot: ChiemO() (DangCoXe = true)
            PM->>PM: VeDangGui.push_back(move(ve))
            PM-->>Main: const ParkingTicket& (vé vừa tạo)
            Main->>User: In chi tiết vé + Cảnh báo nếu bãi >= 80%
            Main->>PM: XuatFile(duongDanChinh) (Tự động lưu)
            PM->>Storage: Luu(*this, duongDanChinh)
            Storage-->>Main: Ghi file tạm .tmp và thay thế file chính
        end
    end
```

#### Kịch bản 2: Quy trình Trả Xe và Tính Phí Đa Hình (`TraXe`)

```mermaid
sequenceDiagram
    autonumber
    actor User as Người dùng (Console)
    participant Main as main.cpp
    participant PM as ParkingManager
    participant Ticket as ParkingTicket
    participant Policy as FeePolicy (Hourly / Flat)
    participant Slot as ParkingSlot
    participant Storage as ParkingFileStorage

    User->>Main: Chọn Menu 3 (Nhập mã vé, phút ra)
    Main->>PM: TraXe(maVe, phutRa)
    PM->>PM: Tìm maVe trong VeDangGui
    alt Không tìm thấy trong VeDangGui
        PM->>PM: TimVe(maVe) trong LichSuVe
        PM-->>Main: throw invalid_argument (Vé đã trả hoặc không tồn tại)
        Main-->>User: Hiển thị lỗi, giữ nguyên trạng thái
    else Tìm thấy vé đang hoạt động
        PM->>Ticket: GetPhutVao(), GetMaO(), GetXe().GetLoai()
        PM->>PM: Kiểm tra phutRa >= PhutVao & TimOTheoMa(MaO)
        PM->>Policy: ChinhSachPhi->TinhPhi(loaiXe, phutRa - phutVao)
        Note over PM,Policy: Đa hình động (Runtime Polymorphism): Gọi HourlyFeePolicy hoặc FlatFeePolicy
        Policy-->>PM: phi (VND)
        PM->>PM: LichSuVe.reserve(LichSuVe.size() + 1)
        PM->>Ticket: DongVe(phutRa, phi) (KetThuc = true)
        PM->>Slot: GiaiPhongO() (DangCoXe = false)
        PM->>PM: Chuyển unique_ptr từ VeDangGui sang LichSuVe
        PM->>PM: DoanhThu += phi
        PM-->>Main: const ParkingTicket& (vé đã đóng)
        Main->>User: In thông tin vé, thời gian gửi và số tiền thanh toán
        Main->>PM: XuatFile(duongDanChinh) (Tự động lưu)
        PM->>Storage: Luu(*this, duongDanChinh)
    end
```

#### Kịch bản 3: Quy trình Nạp File An Toàn (`ParkingFileStorage::Nap`)

```mermaid
sequenceDiagram
    autonumber
    participant Main as main.cpp
    participant PM as ParkingManager (Hiện tại)
    participant Storage as ParkingFileStorage
    participant TempPM as ParkingManager (duLieuMoi)
    participant Ticket as ParkingTicket

    Main->>PM: NhapFile(duongDan)
    PM->>Storage: Nap(*this, duongDan)
    Storage->>Storage: KiemTraDuongDan(duongDan) & mở ifstream
    Storage->>TempPM: Khởi tạo duLieuMoi (10 ô mặc định)
    loop Đọc từng dòng CSV (Giai đoạn 0 -> 5)
        Storage->>Storage: TachCSV(dong)
        Note over Storage,TempPM: GĐ 0: PARKING_OOP,1 | GĐ 1: POLICY | GĐ 2: COUNTER | GĐ 3: 10 SLOT | GĐ 4: TICKET | GĐ 5: END
        Storage->>Ticket: new ParkingTicket(xe, maO, phutVao, maVe, phutRa, phi, daKetThuc)
        Storage->>TempPM: Đưa vào VeDangGui (và ChiemO) hoặc LichSuVe
    end
    alt Có bất kỳ lỗi định dạng hoặc logic nào
        Storage-->>Main: throw runtime_error("Loi du lieu dong X...")
        Note over PM,TempPM: duLieuMoi tự hủy (RAII), PM hiện tại hoàn toàn không bị ảnh hưởng
    else Toàn bộ file hợp lệ (Đạt Giai đoạn 5 & EOF)
        Storage->>PM: swap(DanhSachO, VeDangGui, LichSuVe, ChinhSachPhi)
        Storage->>PM: DoanhThu = duLieuMoi.DoanhThu
        Storage->>Ticket: SoVeDaTao = max(SoVeDaTao, boDemDaLuu)
        Storage-->>Main: Hoàn tất nạp dữ liệu (Commit không ném ngoại lệ)
    end
```

---

### 1.3. Sơ Đồ Trạng Thái (UML State Diagrams)

```mermaid
stateDiagram-v2
    direction LR

    state "Vòng đời Ô Đỗ (ParkingSlot)" as SlotLifecycle {
        [*] --> Trong : Khởi tạo ParkingSlot(maO, loaiO, khoangCach)
        Trong : TRỐNG (DangCoXe = false)
        DaCoXe : ĐÃ CÓ XE (DangCoXe = true)

        Trong --> DaCoXe : ChiemO() [Khi NhanXe hoặc NapFile có vé ACTIVE]
        DaCoXe --> Trong : GiaiPhongO() [Khi TraXe thành công]
        DaCoXe --> DaCoXe : ChiemO() -> throw logic_error
        Trong --> Trong : GiaiPhongO() -> throw logic_error
    }

    state "Vòng đời Vé Xe (ParkingTicket)" as TicketLifecycle {
        [*] --> DangGui : NhanXe() tạo vé mới (++SoVeDaTao)
        [*] --> DangGui : NapFile() gặp dòng TICKET,...,ACTIVE
        [*] --> DaKetThuc : NapFile() gặp dòng TICKET,...,CLOSED

        DangGui : ĐANG GỬI / ACTIVE (KetThuc = false, PhutRa = -1, Phi = 0)
        DaKetThuc : ĐÃ TRẢ / CLOSED (KetThuc = true, PhutRa >= PhutVao, Phi >= 0)

        DangGui --> DaKetThuc : DongVe(phutRa, phi) [Chuyển từ VeDangGui sang LichSuVe]
        DaKetThuc --> DaKetThuc : DongVe() lần 2 -> throw logic_error
        DaKetThuc --> [*] : Khi chương trình kết thúc hoặc NhapFile thay thế
    }
```

---

## Phần 2: Sơ Đồ Luồng Dữ Liệu (Dataflow Diagram — DFD)

### 2.1. DFD Mức Ngữ Cảnh (Context Diagram — Level 0)

```mermaid
flowchart LR
    User(["Người vận hành (Bàn phím / Màn hình Console)"])
    System["Hệ Thống Quản Lý Bãi Đỗ Xe (ParkingOOP.exe)"]
    MainDB[("File Dữ Liệu Chính (parking_data.csv)")]
    BackupDB[("File Sao Lưu / Nhập Ngoài (*.csv / *.txt)")]

    User -- "Lựa chọn menu (0..10), Loại xe, Biển số, Mã vé, Phút vào/ra, Đường dẫn file" --> System
    System -- "Bảng danh sách ô, Chi tiết vé, Thống kê & Doanh thu, Cảnh báo >= 80%, Thông báo lỗi" --> User

    MainDB -- "Tự động nạp trạng thái khi khởi động" --> System
    System -- "Tự động ghi (Atomic Save) sau khi Nhận/Trả/Đổi phí/Nhập file" --> MainDB

    System -- "Menu 9: Xuất toàn bộ snapshot" --> BackupDB
    BackupDB -- "Menu 10: Đọc & kiểm tra toàn vẹn để thay thế dữ liệu" --> System
```

---

### 2.2. DFD Mức Đỉnh (Level 1 — Kiến Trúc Luồng Dữ Liệu Toàn Hệ Thống)

```mermaid
flowchart TB
    User(["Người dùng Console"])

    subgraph UI_Layer ["Tầng Giao Diện (main.cpp)"]
        P1["P1: Bộ đọc & Kiểm tra đầu vào (NhapDong, NhapSoNguyen)"]
        P2["P2: Bộ định tuyến Menu & Hiển thị (HienThiMenu, XemDanhSachO, HienThiChiTietVe, XemThongKe)"]
    end

    subgraph Business_Layer ["Tầng Nghiệp Vụ (ParkingManager, Vehicle, FeePolicy)"]
        P3["P3: Nghiệp vụ Nhận Xe & Phân Ô (NhanXe, ChuanHoaBienSo, ChonO)"]
        P4["P4: Nghiệp vụ Trả Xe & Tính Phí (TraXe, FeePolicy::TinhPhi, DongVe)"]
        P5["P5: Tra Cứu, Thống Kê & Đổi Chính Sách (TimVe, TimXeDangGui, ThongKe, DoiChinhSachPhi)"]
    end

    subgraph RAM_Storage ["Kho Dữ Liệu Trong Bộ Nhớ (RAM State)"]
        D1[("D1: DanhSachO - vector<ParkingSlot>")]
        D2[("D2: VeDangGui - vector<unique_ptr<ParkingTicket>>")]
        D3[("D3: LichSuVe - vector<unique_ptr<ParkingTicket>> + DoanhThu")]
        D4[("D4: ChinhSachPhi - unique_ptr<FeePolicy>")]
        D5[("D5: Bộ đếm tĩnh ParkingTicket::SoVeDaTao")]
    end

    subgraph Persistence_Layer ["Tầng Lưu Trữ Bền Vững (ParkingFileStorage)"]
        P6["P6: Bộ Tuần Tự Hóa & Giải Tuần Tự CSV (ParkingFileStorage::Luu & Nap)"]
        D6[("D6: File CSV / TXT trên Đĩa (parking_data.csv, du_lieu_mau.csv)")]
    end

    User -- "Chuỗi ký tự bàn phím (stdin)" --> P1
    P1 -- "Dữ liệu đã ép kiểu & kiểm tra biên" --> P2
    P2 -- "Bảng định dạng, Cảnh báo, Lỗi (stdout/stderr)" --> User

    P2 -- "loai, bienSo, phutVao" --> P3
    P3 -- "Chuẩn hóa & kiểm tra trùng biển số" --> D2
    P3 -- "Tìm ô trống gần cổng nhất (operator<) & gọi ChiemO()" --> D1
    P3 -- "Cấp mã vé mới (++SoVeDaTao)" --> D5
    P3 -- "Thêm vé ACTIVE mới" --> D2
    P3 -- "Trả về ParkingTicket&" --> P2

    P2 -- "maVe, phutRa" --> P4
    P4 -- "Tìm vé ACTIVE theo maVe" --> D2
    P4 -- "Đọc công thức tính phí hiện hành" --> D4
    P4 -- "Gọi GiaiPhongO()" --> D1
    P4 -- "Đóng vé, chuyển sang LichSuVe & cộng DoanhThu" --> D3
    P4 -- "Trả về ParkingTicket& đã đóng" --> P2

    P2 -- "Yêu cầu tra cứu / thống kê / đổi phí" --> P5
    D1 & D2 & D3 & D4 --> P5
    P5 -- "Cập nhật chính sách mới" --> D4
    P5 -- "ThongKeBaiXe / ParkingTicket*" --> P2

    P2 -- "Kích hoạt lưu tự động (daSuaDoi = true) hoặc Menu 9 / Menu 10" --> P6
    D1 & D2 & D3 & D4 & D5 <--> P6
    P6 <-- "Đọc trực tiếp / Ghi qua file tạm .tmp" --> D6
```

---

### 2.3. DFD Mức Chi Tiết (Level 2 — Các Luồng Biến Đổi Dữ Liệu Trọng Tâm)

#### 2.3.1. Luồng dữ liệu chi tiết: Nhận Xe & Tự Động Chọn Ô (`ParkingManager::NhanXe`)

```mermaid
flowchart TD
    In(["Input từ main.cpp: loai (VehicleType), bienSo (string), phutVao (int)"])
    V1{"1. Kiểm tra loai hợp lệ & phutVao >= 0?"}
    Norm["2. Vehicle::ChuanHoaBienSo(bienSo): Bỏ khoảng trắng, từ chối ký tự điều khiển, chuyển IN HOA"]
    V2{"3. Chuỗi sau chuẩn hóa có rỗng không?"}
    Dup{"4. TimXeDangGui(bienSoChuan) có khác NULL không?"}
    Select["5. ChonO(loai): Duyệt DanhSachO[0..9], lọc ô trống cùng loại, chọn ô ưu tiên nhất qua ParkingSlot::operator<"]
    V3{"6. Tìm được ô trống (o != NULL)?"}
    CreateObj["7. Khởi tạo đối tượng trong RAM: unique_ptr<Vehicle> (Motorbike/Car) & unique_ptr<ParkingTicket> (++SoVeDaTao)"]
    Commit["8. Cập nhật trạng thái: o->ChiemO() (DangCoXe = true) & VeDangGui.push_back(move(ve))"]
    AutoSave["9. Tự động lưu file chính: ThuLuuDuLieu() -> ParkingFileStorage::Luu()"]
    Err(["Ném ngoại lệ (invalid_argument / runtime_error) - Trạng thái bãi xe giữ nguyên 100%"])
    Out(["Output: Hiển thị chi tiết ParkingTicket & Cảnh báo nếu TyLeSuDung >= 80%"])

    In --> V1
    V1 -- "Sai" --> Err
    V1 -- "Đúng" --> Norm --> V2
    V2 -- "Rỗng" --> Err
    V2 -- "Hợp lệ" --> Dup
    Dup -- "Đã tồn tại xe đang gửi" --> Err
    Dup -- "Chưa tồn tại" --> Select --> V3
    V3 -- "Hết chỗ (NULL)" --> Err
    V3 -- "Có ô (oTotNhat)" --> CreateObj --> Commit --> AutoSave --> Out
```

#### 2.3.2. Luồng dữ liệu chi tiết: Trả Xe & Tính Phí Đa Hình (`ParkingManager::TraXe`)

```mermaid
flowchart TD
    In(["Input từ main.cpp: maVe (int), phutRa (int)"])
    FindActive{"1. Tìm maVe trong vector VeDangGui?"}
    FindHistory{"2. Tìm maVe trong LichSuVe (TimVe)?"}
    ErrClosed(["Throw invalid_argument: Vé này đã kết thúc, không thể trả lần nữa"])
    ErrNotFound(["Throw invalid_argument: Không tồn tại mã vé này"])
    CheckTime{"3. Kiểm tra thời gian: phutRa >= ve.GetPhutVao()?"}
    ErrTime(["Throw invalid_argument: Phút ra phải >= phút vào"])
    CheckSlot{"4. TimOTheoMa(ve.GetMaO()) có tồn tại và đang CoXe()?"}
    ErrSlot(["Throw logic_error: Trạng thái ô đỗ không khớp"])
    CalcFee["5. Tính phí đa hình: phi = ChinhSachPhi->TinhPhi(loai, phutRa - PhutVao)"]
    CheckOverflow{"6. Kiểm tra phi >= 0 và DoanhThu + phi không tràn long long?"}
    ErrOverflow(["Throw invalid_argument / overflow_error"])
    Reserve["7. Cấp phát trước vùng nhớ lịch sử: LichSuVe.reserve(LichSuVe.size() + 1)"]
    Commit["8. Commit trạng thái: ve.DongVe(phutRa, phi), o->GiaiPhongO(), chuyển vé sang LichSuVe, DoanhThu += phi"]
    AutoSave["9. Tự động lưu file chính: ThuLuuDuLieu() -> ParkingFileStorage::Luu()"]
    Out(["Output: In biên lai thanh toán & giải phóng ô đỗ"])

    In --> FindActive
    FindActive -- "Không thấy" --> FindHistory
    FindHistory -- "Thấy trong lịch sử" --> ErrClosed
    FindHistory -- "Không thấy" --> ErrNotFound
    FindActive -- "Tìm thấy tại VeDangGui[viTri]" --> CheckTime
    CheckTime -- "Sai" --> ErrTime
    CheckTime -- "Đúng" --> CheckSlot
    CheckSlot -- "Sai" --> ErrSlot
    CheckSlot -- "Đúng" --> CalcFee --> CheckOverflow
    CheckOverflow -- "Tràn số / Âm" --> ErrOverflow
    CheckOverflow -- "An toàn" --> Reserve --> Commit --> AutoSave --> Out
```

#### 2.3.3. Luồng dữ liệu Đọc & Ghi File CSV (`ParkingFileStorage`)

```mermaid
flowchart LR
    subgraph Luu_File ["Luồng Ghi File An Toàn (ParkingFileStorage::Luu)"]
        L1["Đọc trạng thái ParkingManager"]
        L2["Tạo chuỗi CSV trong bộ nhớ (ostringstream): PARKING_OOP,1 -> POLICY -> COUNTER -> 10 SLOT -> TICKET -> END"]
        L3["GhiThayTheAnToan(): Tạo file tạm (.tmp), WriteFile + FlushFileBuffers, Đóng file"]
        L4["Atomic Replace: MoveFileExA / std::rename sang file đích"]
        L1 --> L2 --> L3 --> L4
    end

    subgraph Nap_File ["Luồng Nạp File Giao Dịch (ParkingFileStorage::Nap)"]
        N1["Mở file nhị phân (ifstream) & cắt UTF-8 BOM / CRLF"]
        N2["Máy trạng thái 5 giai đoạn (giaiDoan 0..5): Kiểm tra từng dòng CSV vào bản tạm duLieuMoi"]
        N3{"Toàn bộ file hợp lệ?"}
        N4["Commit bằng vector::swap() & cập nhật SoVeDaTao = max(cũ, mới)"]
        N5["Hủy bản tạm duLieuMoi, giữ nguyên dữ liệu đang chạy"]
        N1 --> N2 --> N3
        N3 -- "Hợp lệ" --> N4
        N3 -- "Có lỗi" --> N5
    end
```

---

### 2.4. Bảng Tổng Hợp Luồng Dữ Liệu Theo Chức Năng Menu

| Menu | Chức năng | Dữ liệu đầu vào (Input) | Lớp & Phương thức xử lý chính | Kho dữ liệu tác động | Dữ liệu đầu ra (Output) & Lưu trữ |
|---:|---|---|---|---|---|
| **Khởi động** | Nạp hoặc khởi tạo file chính | `argc, argv` (đường dẫn file tùy chọn) | `LayDuongDanChinh`, `ParkingManager::NhapFile` hoặc `XuatFile` | Khởi tạo `DanhSachO`, nạp `VeDangGui`, `LichSuVe`, `ChinhSachPhi`, `DoanhThu`, `SoVeDaTao` | Thông báo trạng thái khởi tạo file `parking_data.csv` |
| **1** | Xem danh sách ô đỗ | Lựa chọn `1` | `XemDanhSachO` -> `LayDanhSachO`, `ThongKe` | Chỉ đọc `DanhSachO` (`const`) | Bảng 10 ô đỗ + Cảnh báo nếu >= 80% |
| **2** | Nhận xe vào bãi | Loại xe (`1`/`2`), Biển số, Phút vào | `NhanXe` -> `ChuanHoaBienSo`, `ChonO`, `ChiemO` | Ghi `DanhSachO`, thêm `VeDangGui`, tăng `SoVeDaTao` | In chi tiết vé mới + Tự động lưu file chính |
| **3** | Trả xe và tính phí | Mã vé, Phút ra | `TraXe` -> `FeePolicy::TinhPhi`, `DongVe`, `GiaiPhongO` | Giải phóng ô trong `DanhSachO`, chuyển vé từ `VeDangGui` sang `LichSuVe`, cộng `DoanhThu` | In biên lai phí + Tự động lưu file chính |
| **4** | Tra cứu vé / biển số | Kiểu tra cứu (`1`: Mã vé, `2`: Biển số) | `TraCuu` -> `TimVe` hoặc `TimXeDangGui` | Chỉ đọc `VeDangGui` và `LichSuVe` (`const`) | Chi tiết vé tìm thấy hoặc báo không tìm thấy |
| **5** | Danh sách xe đang gửi | Lựa chọn `5` | `LayVeDangGui` -> `HienThiDanhSachVe` | Chỉ đọc `VeDangGui` (`const`) | Bảng các xe đang gửi trong bãi |
| **6** | Thống kê và doanh thu | Lựa chọn `6` | `XemThongKe` -> `ParkingManager::ThongKe` | Chỉ đọc `DanhSachO`, `LichSuVe`, `DoanhThu`, `ChinhSachPhi` | Bảng thống kê theo loại xe, doanh thu, cảnh báo >= 80% |
| **7** | Đổi chính sách tính phí | Lựa chọn (`1`: Theo giờ, `2`: Theo lượt) | `DoiChinhSach` -> `DoiChinhSachPhi` | Thay thế con trỏ `ChinhSachPhi` (`unique_ptr<FeePolicy>`) | Thông báo chính sách mới + Tự động lưu file chính |
| **8** | Lịch sử vé đã trả | Lựa chọn `8` | `LayLichSuVe` -> `HienThiDanhSachVe` | Chỉ đọc `LichSuVe` (`const`) | Bảng lịch sử các lượt gửi đã hoàn tất và phí |
| **9** | Xuất file CSV / TXT | Đường dẫn file xuất | `XuatDuLieu` -> `ParkingFileStorage::Luu` | Chỉ đọc toàn bộ trạng thái RAM, ghi ra file đích | Tạo file sao lưu CSV/TXT an toàn |
| **10** | Nhập file CSV / TXT | Đường dẫn file nhập | `NhapDuLieu` -> `ParkingFileStorage::Nap` | Thay thế toàn bộ `DanhSachO`, `VeDangGui`, `LichSuVe`, `ChinhSachPhi`, `DoanhThu`; nâng `SoVeDaTao` | Thay thế dữ liệu trong RAM + Tự động lưu vào file chính |
| **0 / EOF** | Thoát chương trình | Lựa chọn `0` hoặc `Ctrl+Z` / `Ctrl+D` | `KetThucChuongTrinh` -> `ThuLuuDuLieu` | Lưu các thay đổi chưa lưu (`chuaLuu == true`) nếu có | Giải phóng bộ nhớ tự động qua destructor / RAII, thoát với mã `0` (hoặc `1` nếu lỗi ghi file) |
