#include "ParkingFileStorage.h"
#include "ParkingManager.h"

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <fstream>
#include <limits>
#include <locale>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace {

void BaoLoiDong(std::size_t soDong, const std::string& noiDung) {
    std::ostringstream loi;
    loi << "Loi du lieu dong " << soDong << ": " << noiDung;
    throw std::runtime_error(loi.str());
}

bool DongTrong(const std::string& dong) {
    return dong.find_first_not_of(" \t\r") == std::string::npos;
}

void KiemTraDuongDan(const std::string& duongDan) {
    if (duongDan.empty() || DongTrong(duongDan)) {
        throw std::invalid_argument("Duong dan file khong duoc de trong.");
    }
    for (std::size_t i = 0; i < duongDan.size(); ++i) {
        if (static_cast<unsigned char>(duongDan[i]) < 32) {
            throw std::invalid_argument("Duong dan file chua ky tu khong hop le.");
        }
    }
#ifdef _WIN32
    const DWORD thuocTinh = GetFileAttributesA(duongDan.c_str());
    if (thuocTinh != INVALID_FILE_ATTRIBUTES &&
        (thuocTinh & FILE_ATTRIBUTE_DIRECTORY) != 0) {
        throw std::invalid_argument("Duong dan dang tro toi thu muc, khong phai file.");
    }
#else
    struct stat thongTin;
    if (stat(duongDan.c_str(), &thongTin) == 0 && S_ISDIR(thongTin.st_mode)) {
        throw std::invalid_argument("Duong dan dang tro toi thu muc, khong phai file.");
    }
#endif
}

std::vector<std::string> TachCSV(const std::string& dong, std::size_t soDong) {
    std::vector<std::string> cacTruong;
    std::size_t viTri = 0;
    for (;;) {
        std::string truong;
        if (viTri < dong.size() && dong[viTri] == '"') {
            ++viTri;
            bool daDongNgoac = false;
            while (viTri < dong.size()) {
                const char kyTu = dong[viTri++];
                if (kyTu != '"') {
                    truong += kyTu;
                } else if (viTri < dong.size() && dong[viTri] == '"') {
                    truong += '"';
                    ++viTri;
                } else {
                    daDongNgoac = true;
                    break;
                }
            }
            if (!daDongNgoac || (viTri < dong.size() && dong[viTri] != ',')) {
                BaoLoiDong(soDong, "Dau ngoac kep CSV khong hop le.");
            }
        } else {
            while (viTri < dong.size() && dong[viTri] != ',') {
                if (dong[viTri] == '"') {
                    BaoLoiDong(soDong, "Dau ngoac kep phai bat dau mot truong CSV.");
                }
                truong += dong[viTri++];
            }
        }
        cacTruong.push_back(truong);
        if (viTri == dong.size()) {
            break;
        }
        ++viTri; // Bo dau phay; vong lap tiep theo van giu truong rong cuoi dong.
    }
    return cacTruong;
}

void KiemTraSoTruong(const std::vector<std::string>& truong,
                     std::size_t soLuong, std::size_t soDong) {
    if (truong.size() != soLuong) {
        BaoLoiDong(soDong, "So truong CSV khong dung dinh dang.");
    }
}

long long DocSoKhongAm(const std::string& chuoi, long long gioiHan,
                      std::size_t soDong) {
    if (chuoi.empty()) {
        BaoLoiDong(soDong, "Truong so khong duoc rong.");
    }
    long long ketQua = 0;
    for (std::size_t i = 0; i < chuoi.size(); ++i) {
        if (chuoi[i] < '0' || chuoi[i] > '9') {
            BaoLoiDong(soDong, "So phai chi gom cac chu so thap phan khong am.");
        }
        const int chuSo = chuoi[i] - '0';
        if (ketQua > (gioiHan - chuSo) / 10) {
            BaoLoiDong(soDong, "Gia tri so vuot gioi han luu tru.");
        }
        ketQua = ketQua * 10 + chuSo;
    }
    return ketQua;
}

int DocInt(const std::string& chuoi, std::size_t soDong, bool phaiDuong = false) {
    const int ketQua = static_cast<int>(
        DocSoKhongAm(chuoi, std::numeric_limits<int>::max(), soDong));
    if (phaiDuong && ketQua == 0) {
        BaoLoiDong(soDong, "Ma ve va ma o phai la so nguyen duong.");
    }
    return ketQua;
}

VehicleType DocLoaiXe(const std::string& chuoi, std::size_t soDong) {
    if (chuoi == "MOTORBIKE") {
        return VehicleType::XeMay;
    }
    if (chuoi == "CAR") {
        return VehicleType::OTo;
    }
    BaoLoiDong(soDong, "Loai xe khong duoc ho tro.");
    return VehicleType::XeMay;
}

const char* MaLoaiXe(VehicleType loai) {
    if (loai == VehicleType::XeMay) {
        return "MOTORBIKE";
    }
    if (loai == VehicleType::OTo) {
        return "CAR";
    }
    throw std::invalid_argument("Khong the luu loai xe khong hop le.");
}

std::string MaHoaCSV(const std::string& truong) {
    if (truong.find_first_of(",\"") == std::string::npos) {
        return truong;
    }
    std::string ketQua = "\"";
    for (std::size_t i = 0; i < truong.size(); ++i) {
        if (truong[i] == '"') {
            ketQua += '"';
        }
        ketQua += truong[i];
    }
    return ketQua + '"';
}

void GhiVe(std::ostream& out, const ParkingTicket& ve) {
    out << "TICKET," << ve.GetMaVe() << ',' << MaLoaiXe(ve.GetXe().GetLoai())
        << ',' << MaHoaCSV(ve.GetXe().GetBienSo()) << ',' << ve.GetMaO()
        << ',' << ve.GetPhutVao() << ',' << ve.GetPhutRa() << ',' << ve.GetPhi()
        << ',' << (ve.DaKetThuc() ? "CLOSED" : "ACTIVE") << '\n';
}

// Chi thay file dich sau khi file tam da duoc ghi va dong thanh cong.
void GhiThayTheAnToan(const std::string& duongDan, const std::string& noiDung) {
    static unsigned long lanGhi = 0;
    std::string fileTam;
#ifdef _WIN32
    HANDLE file = INVALID_HANDLE_VALUE;
    for (int thu = 0; thu < 100; ++thu) {
        std::ostringstream ten;
        ten << duongDan << ".parking-" << GetCurrentProcessId() << '-'
            << GetTickCount() << '-' << ++lanGhi << ".tmp";
        fileTam = ten.str();
        file = CreateFileA(fileTam.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW,
                           FILE_ATTRIBUTE_NORMAL, NULL);
        if (file != INVALID_HANDLE_VALUE) {
            break;
        }
        if (GetLastError() != ERROR_FILE_EXISTS &&
            GetLastError() != ERROR_ALREADY_EXISTS) {
            throw std::runtime_error("Khong tao duoc file tam. Hay kiem tra duong dan va quyen ghi.");
        }
    }
    if (file == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Khong tao duoc ten file tam duy nhat.");
    }
    try {
        std::size_t viTri = 0;
        while (viTri < noiDung.size()) {
            const std::size_t conLai = noiDung.size() - viTri;
            const DWORD canGhi = static_cast<DWORD>(
                (std::min)(conLai, static_cast<std::size_t>(1024 * 1024)));
            DWORD daGhi = 0;
            if (!WriteFile(file, noiDung.data() + viTri, canGhi, &daGhi, NULL) ||
                daGhi == 0) {
                throw std::runtime_error("Khong ghi duoc du lieu vao file tam.");
            }
            viTri += daGhi;
        }
        if (!FlushFileBuffers(file)) {
            throw std::runtime_error("Khong hoan tat duoc viec ghi file tam.");
        }
        const BOOL daDong = CloseHandle(file);
        file = INVALID_HANDLE_VALUE;
        if (!daDong) {
            throw std::runtime_error("Khong dong duoc file tam sau khi ghi.");
        }
        if (!MoveFileExA(fileTam.c_str(), duongDan.c_str(),
                         MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
            throw std::runtime_error("Khong thay the duoc file dich; file cu duoc giu nguyen.");
        }
    } catch (...) {
        if (file != INVALID_HANDLE_VALUE) {
            CloseHandle(file);
        }
        DeleteFileA(fileTam.c_str());
        throw;
    }
#else
    int file = -1;
    for (int thu = 0; thu < 100; ++thu) {
        std::ostringstream ten;
        ten << duongDan << ".parking-" << getpid() << '-' << ++lanGhi << ".tmp";
        fileTam = ten.str();
        file = open(fileTam.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0600);
        if (file >= 0) {
            break;
        }
        if (errno != EEXIST) {
            throw std::runtime_error("Khong tao duoc file tam. Hay kiem tra duong dan va quyen ghi.");
        }
    }
    if (file < 0) {
        throw std::runtime_error("Khong tao duoc ten file tam duy nhat.");
    }
    try {
        std::size_t viTri = 0;
        while (viTri < noiDung.size()) {
            const ssize_t daGhi = write(file, noiDung.data() + viTri,
                (std::min)(noiDung.size() - viTri, static_cast<std::size_t>(1024 * 1024)));
            if (daGhi < 0 && errno == EINTR) {
                continue;
            }
            if (daGhi <= 0) {
                throw std::runtime_error("Khong ghi duoc du lieu vao file tam.");
            }
            viTri += static_cast<std::size_t>(daGhi);
        }
        if (fsync(file) != 0) {
            throw std::runtime_error("Khong hoan tat duoc viec ghi file tam.");
        }
        const int ketQuaDong = close(file);
        file = -1;
        if (ketQuaDong != 0) {
            throw std::runtime_error("Khong dong duoc file tam sau khi ghi.");
        }
        if (std::rename(fileTam.c_str(), duongDan.c_str()) != 0) {
            throw std::runtime_error("Khong thay the duoc file dich; file cu duoc giu nguyen.");
        }
    } catch (...) {
        if (file >= 0) {
            close(file);
        }
        std::remove(fileTam.c_str());
        throw;
    }
#endif
}

} // namespace

void ParkingFileStorage::Luu(const ParkingManager& quanLy,
                            const std::string& duongDan) {
    KiemTraDuongDan(duongDan);
    const char* chinhSach = NULL;
    if (typeid(quanLy.LayChinhSachPhi()) == typeid(HourlyFeePolicy)) {
        chinhSach = "HOURLY";
    } else if (typeid(quanLy.LayChinhSachPhi()) == typeid(FlatFeePolicy)) {
        chinhSach = "FLAT";
    } else {
        throw std::invalid_argument("Chi luu duoc chinh sach theo gio hoac theo luot.");
    }

    std::ostringstream out;
    out.imbue(std::locale::classic());
    out << "PARKING_OOP,1\nPOLICY," << chinhSach
        << "\nCOUNTER," << ParkingTicket::GetSoVeDaTao() << '\n';
    for (std::size_t i = 0; i < quanLy.DanhSachO.size(); ++i) {
        const ParkingSlot& o = quanLy.DanhSachO[i];
        out << "SLOT," << o.GetMaO() << ',' << MaLoaiXe(o.GetLoaiO())
            << ',' << o.GetKhoangCach() << '\n';
    }
    for (std::size_t i = 0; i < quanLy.VeDangGui.size(); ++i) {
        GhiVe(out, *quanLy.VeDangGui[i]);
    }
    for (std::size_t i = 0; i < quanLy.LichSuVe.size(); ++i) {
        GhiVe(out, *quanLy.LichSuVe[i]);
    }
    out << "END," << quanLy.DanhSachO.size() << ','
        << quanLy.VeDangGui.size() + quanLy.LichSuVe.size() << ','
        << quanLy.DoanhThu << '\n';
    if (!out) {
        throw std::runtime_error("Khong tao duoc du lieu de luu file.");
    }
    GhiThayTheAnToan(duongDan, out.str());
}

void ParkingFileStorage::Nap(ParkingManager& quanLy,
                            const std::string& duongDan) {
    KiemTraDuongDan(duongDan);
    std::ifstream in(duongDan.c_str(), std::ios::binary);
    if (!in) {
        throw std::runtime_error("Khong mo duoc file de nap du lieu.");
    }

    // Phuc hoi hoan toan tren ban tam, khong cap ma ve moi khi doc file.
    ParkingManager duLieuMoi;
    std::set<int> cacMaO;
    std::set<int> cacMaVe;
    std::set<int> cacOCoXe;
    std::set<std::string> cacBienSoDangGui;
    int giaiDoan = 0; // 0: dau file, 1: policy, 2: counter, 3: slot, 4: ticket, 5: end.
    int boDemDaLuu = 0;
    int maVeLonNhat = 0;
    long long tongPhiDaThu = 0;
    std::size_t soDong = 0;
    std::string dong;
    while (std::getline(in, dong)) {
        ++soDong;
        if (soDong == 1 && dong.size() >= 3 &&
            static_cast<unsigned char>(dong[0]) == 0xEF &&
            static_cast<unsigned char>(dong[1]) == 0xBB &&
            static_cast<unsigned char>(dong[2]) == 0xBF) {
            dong.erase(0, 3);
        }
        if (!dong.empty() && dong[dong.size() - 1] == '\r') {
            dong.erase(dong.size() - 1);
        }
        if (DongTrong(dong)) {
            continue;
        }
        if (giaiDoan == 5) {
            BaoLoiDong(soDong, "Co du lieu thua sau ban ghi END.");
        }
        const std::vector<std::string> truong = TachCSV(dong, soDong);
        if (giaiDoan == 0) {
            KiemTraSoTruong(truong, 2, soDong);
            if (truong[0] != "PARKING_OOP" || truong[1] != "1") {
                BaoLoiDong(soDong, "Sai dinh dang hoac phien ban file.");
            }
            giaiDoan = 1;
        } else if (giaiDoan == 1) {
            KiemTraSoTruong(truong, 2, soDong);
            if (truong[0] != "POLICY") {
                BaoLoiDong(soDong, "Thieu ban ghi POLICY.");
            }
            if (truong[1] == "HOURLY") {
                duLieuMoi.ChinhSachPhi.reset(new HourlyFeePolicy());
            } else if (truong[1] == "FLAT") {
                duLieuMoi.ChinhSachPhi.reset(new FlatFeePolicy());
            } else {
                BaoLoiDong(soDong, "Chinh sach tinh phi khong duoc ho tro.");
            }
            giaiDoan = 2;
        } else if (giaiDoan == 2) {
            KiemTraSoTruong(truong, 2, soDong);
            if (truong[0] != "COUNTER") {
                BaoLoiDong(soDong, "Thieu ban ghi COUNTER.");
            }
            boDemDaLuu = DocInt(truong[1], soDong);
            giaiDoan = 3;
        } else if (truong[0] == "SLOT" && giaiDoan == 3) {
            KiemTraSoTruong(truong, 4, soDong);
            const int maO = DocInt(truong[1], soDong, true);
            const VehicleType loai = DocLoaiXe(truong[2], soDong);
            const int khoangCach = DocInt(truong[3], soDong);
            ParkingSlot* o = duLieuMoi.TimOTheoMa(maO);
            if (o == NULL || o->GetLoaiO() != loai ||
                o->GetKhoangCach() != khoangCach) {
                BaoLoiDong(soDong, "O do khong khop cau hinh 6 o xe may va 4 o o to.");
            }
            if (!cacMaO.insert(maO).second) {
                BaoLoiDong(soDong, "Ma o do bi trung.");
            }
        } else if (truong[0] == "TICKET") {
            KiemTraSoTruong(truong, 9, soDong);
            if (cacMaO.size() != duLieuMoi.DanhSachO.size()) {
                BaoLoiDong(soDong, "Phai co du 10 o do truoc cac ve.");
            }
            giaiDoan = 4;
            const int maVe = DocInt(truong[1], soDong, true);
            const VehicleType loai = DocLoaiXe(truong[2], soDong);
            std::string bienSo;
            try {
                bienSo = Vehicle::ChuanHoaBienSo(truong[3]);
            } catch (const std::invalid_argument& loi) {
                BaoLoiDong(soDong, loi.what());
            }
            const int maO = DocInt(truong[4], soDong, true);
            const int phutVao = DocInt(truong[5], soDong);
            const int phutRa = truong[6] == "-1" ? -1 : DocInt(truong[6], soDong);
            const long long phi = DocSoKhongAm(
                truong[7], std::numeric_limits<long long>::max(), soDong);
            if (truong[8] != "ACTIVE" && truong[8] != "CLOSED") {
                BaoLoiDong(soDong, "Trang thai ve khong duoc ho tro.");
            }
            const bool daKetThuc = truong[8] == "CLOSED";
            ParkingSlot* o = duLieuMoi.TimOTheoMa(maO);
            if (o == NULL || o->GetLoaiO() != loai) {
                BaoLoiDong(soDong, "Ma o hoac loai o khong hop le cho ve.");
            }
            if (!cacMaVe.insert(maVe).second) {
                BaoLoiDong(soDong, "Ma ve bi trung.");
            }
            if (maVe > boDemDaLuu) {
                BaoLoiDong(soDong, "Bo dem ve nho hon ma ve da luu.");
            }
            maVeLonNhat = (std::max)(maVeLonNhat, maVe);
            if (daKetThuc) {
                if (phutRa < phutVao) {
                    BaoLoiDong(soDong, "Phut ra cua ve da dong nho hon phut vao.");
                }
                if (phi > std::numeric_limits<long long>::max() - tongPhiDaThu) {
                    BaoLoiDong(soDong, "Tong doanh thu vuot gioi han luu tru.");
                }
                tongPhiDaThu += phi;
            } else {
                if (phutRa != -1 || phi != 0) {
                    BaoLoiDong(soDong, "Ve dang gui phai co phut ra -1 va phi 0.");
                }
                if (!cacBienSoDangGui.insert(bienSo).second) {
                    BaoLoiDong(soDong, "Trung bien so giua cac ve dang gui.");
                }
                if (!cacOCoXe.insert(maO).second) {
                    BaoLoiDong(soDong, "Hai ve dang gui cung chiem mot o.");
                }
            }

            std::unique_ptr<Vehicle> xe;
            if (loai == VehicleType::XeMay) {
                xe.reset(new Motorbike(bienSo));
            } else {
                xe.reset(new Car(bienSo));
            }
            std::unique_ptr<ParkingTicket> ve(new ParkingTicket(
                std::move(xe), maO, phutVao, maVe, phutRa, phi, daKetThuc));
            if (daKetThuc) {
                duLieuMoi.LichSuVe.push_back(std::move(ve));
            } else {
                o->ChiemO();
                duLieuMoi.VeDangGui.push_back(std::move(ve));
            }
        } else if (truong[0] == "END") {
            KiemTraSoTruong(truong, 4, soDong);
            const int soO = DocInt(truong[1], soDong);
            const int soVe = DocInt(truong[2], soDong);
            const long long doanhThu = DocSoKhongAm(
                truong[3], std::numeric_limits<long long>::max(), soDong);
            if (cacMaO.size() != duLieuMoi.DanhSachO.size() ||
                static_cast<std::size_t>(soO) != cacMaO.size() ||
                static_cast<std::size_t>(soVe) != cacMaVe.size() ||
                doanhThu != tongPhiDaThu || boDemDaLuu < maVeLonNhat) {
                BaoLoiDong(soDong, "Tong ket END khong khop so o, so ve hoac doanh thu.");
            }
            duLieuMoi.DoanhThu = doanhThu;
            giaiDoan = 5;
        } else {
            BaoLoiDong(soDong, "Ban ghi khong hop le hoac sai thu tu.");
        }
    }
    if (!in.eof()) {
        throw std::runtime_error("Loi khi doc file du lieu.");
    }
    if (giaiDoan != 5) {
        BaoLoiDong(soDong + 1, "File chua hoan chinh; thieu ban ghi END hoac phan dau file.");
    }
    in.clear();
    in.close();
    if (in.fail()) {
        throw std::runtime_error("Khong dong duoc file sau khi doc.");
    }

    // Cac phep swap va gan so nguyen khong nem ngoai le: chi commit mot lan.
    quanLy.DanhSachO.swap(duLieuMoi.DanhSachO);
    quanLy.VeDangGui.swap(duLieuMoi.VeDangGui);
    quanLy.LichSuVe.swap(duLieuMoi.LichSuVe);
    quanLy.ChinhSachPhi.swap(duLieuMoi.ChinhSachPhi);
    quanLy.DoanhThu = duLieuMoi.DoanhThu;
    ParkingTicket::SoVeDaTao = (std::max)(ParkingTicket::SoVeDaTao, boDemDaLuu);
}
