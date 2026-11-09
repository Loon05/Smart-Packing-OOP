#include "ParkingManager.h"
#include "FeePolicy.h"

#include <cerrno>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace {

// EOF o bat ky buoc nhap nao deu dung chuong trinh, khong gui du lieu do dang.
class KetThucNhap : public std::exception {
public:
    const char* what() const noexcept override {
        return "Da ket thuc luong nhap.";
    }
};

std::string NhapDong(const std::string& loiNhac) {
    std::cout << loiNhac;
    std::string dong;
    if (!std::getline(std::cin, dong)) {
        throw KetThucNhap();
    }
    return dong;
}

int NhapSoNguyen(const std::string& loiNhac, int nhoNhat, int lonNhat) {
    for (;;) {
        const std::string dong = NhapDong(loiNhac);
        std::istringstream boDoc(dong);
        int giaTri = 0;
        char kyTuThua = '\0';
        // Doc ca dong: "12abc", "1 2" va so tran int deu bi tu choi.
        if ((boDoc >> giaTri) && !(boDoc >> kyTuThua)
            && giaTri >= nhoNhat && giaTri <= lonNhat) {
            return giaTri;
        }
        std::cout << "Du lieu khong hop le. Hay nhap so nguyen tu "
                  << nhoNhat << " den " << lonNhat << ".\n";
    }
}

std::string DinhDangTyLe(double tyLe) {
    std::ostringstream ketQua;
    ketQua << std::fixed << std::setprecision(1) << tyLe << '%';
    return ketQua.str();
}

std::string DinhDangThoiDiem(int soPhut) {
    std::ostringstream ketQua;
    ketQua << "ngay " << soPhut / 1440 + 1 << ", "
            << std::setfill('0') << std::setw(2) << (soPhut % 1440) / 60
            << ':' << std::setw(2) << soPhut % 60;
    return ketQua.str();
}

void HienThiCanhBao(const ThongKeBaiXe& thongKe) {
    if (thongKe.CanhBaoGanDay) {
        std::cout << "CANH BAO: Bai do da su dung "
                  << DinhDangTyLe(thongKe.TyLeSuDung)
                  << " (tu 80% tro len). Con " << thongKe.OTrong
                  << " o trong.\n";
    }
}

void HienThiChiTietVe(const ParkingTicket& ve) {
    std::cout << ve << '\n';
    std::cout << "Thoi diem vao: " << DinhDangThoiDiem(ve.GetPhutVao()) << '\n';
    if (ve.DaKetThuc()) {
        std::cout << "Thoi diem ra : " << DinhDangThoiDiem(ve.GetPhutRa())
                  << '\n';
    }
}

void HienThiDanhSachVe(const std::vector<const ParkingTicket*>& danhSach,
                      bool laLichSu) {
    if (danhSach.empty()) {
        std::cout << (laLichSu ? "Chua co luot gui xe da ket thuc.\n"
                              : "Hien khong co xe dang gui.\n");
        return;
    }
    std::cout << std::left << std::setw(9) << "Ma ve"
              << std::setw(20) << "Bien so" << std::setw(12) << "Loai xe"
              << std::setw(8) << "Ma o" << std::setw(12) << "Phut vao";
    if (laLichSu) {
        std::cout << std::setw(12) << "Phut ra" << "Phi (VND)";
    }
    std::cout << '\n' << std::string(laLichSu ? 89 : 61, '-') << '\n';
    for (std::size_t i = 0; i < danhSach.size(); ++i) {
        const ParkingTicket& ve = *danhSach[i];
        std::cout << std::left << std::setw(9) << ve.GetMaVe()
                  << std::setw(20) << ve.GetXe().GetBienSo()
                  << std::setw(12) << TenLoaiXe(ve.GetXe().GetLoai())
                  << std::setw(8) << ve.GetMaO() << std::setw(12) << ve.GetPhutVao();
        if (laLichSu) {
            std::cout << std::setw(12) << ve.GetPhutRa() << ve.GetPhi();
        }
        std::cout << '\n';
    }
}

void XemDanhSachO(const ParkingManager& quanLy) {
    const std::vector<ParkingSlot>& danhSach = quanLy.LayDanhSachO();
    std::cout << "\n--- DANH SACH O DO ---\n"
              << std::left << std::setw(10) << "Ma o"
              << std::setw(14) << "Loai o" << std::setw(20) << "Cach cong (m)"
              << "Trang thai\n" << std::string(60, '-') << '\n';
    for (std::size_t i = 0; i < danhSach.size(); ++i) {
        const ParkingSlot& o = danhSach[i];
        std::cout << std::left << std::setw(10) << o.GetMaO()
                  << std::setw(14) << TenLoaiXe(o.GetLoaiO())
                  << std::setw(20) << o.GetKhoangCach()
                  << (o.CoXe() ? "Da co xe" : "Trong") << '\n';
    }
    std::cout << "Xe duoc tu dong phan vao o trong dung loai, gan cong nhat.\n"
              << "Neu cung khoang cach, uu tien ma o nho hon.\n";
    HienThiCanhBao(quanLy.ThongKe());
}

void NhanXe(ParkingManager& quanLy, bool& daSuaDoi) {
    std::cout << "\n--- NHAN XE VAO BAI ---\n1. Xe may\n2. O to\n";
    const int luaChon = NhapSoNguyen("Loai xe: ", 1, 2);
    const std::string bienSo = NhapDong("Bien so: ");
    const int phutVao = NhapSoNguyen("Phut vao (480 = 08:00 ngay 1): ", 0,
                                     std::numeric_limits<int>::max());
    const VehicleType loai = luaChon == 1 ? VehicleType::XeMay : VehicleType::OTo;
    const ParkingTicket& ve = quanLy.NhanXe(loai, bienSo, phutVao);
    daSuaDoi = true;
    std::cout << "Nhan xe thanh cong. Hay giu ma ve de tra xe.\n";
    HienThiChiTietVe(ve);
    HienThiCanhBao(quanLy.ThongKe());
}

void TraXe(ParkingManager& quanLy, bool& daSuaDoi) {
    std::cout << "\n--- TRA XE VA TINH PHI ---\n"
              << "Ap dung chinh sach hien tai khi tra xe: "
              << quanLy.LayChinhSachPhi().GetTen() << '\n';
    const int maVe = NhapSoNguyen("Ma ve: ", 1, std::numeric_limits<int>::max());
    const int phutRa = NhapSoNguyen("Phut ra (qua ngay thi tiep tuc dem): ", 0,
                                   std::numeric_limits<int>::max());
    const ParkingTicket& ve = quanLy.TraXe(maVe, phutRa);
    daSuaDoi = true;
    std::cout << "Tra xe thanh cong. O " << ve.GetMaO() << " da duoc giai phong.\n";
    HienThiChiTietVe(ve);
    std::cout << "So tien thanh toan: " << ve.GetPhi() << " VND\n";
}

void TraCuu(const ParkingManager& quanLy) {
    std::cout << "\n--- TRA CUU ---\n1. Theo ma ve (gom ca lich su)\n"
              << "2. Theo bien so xe dang gui\n0. Quay lai\n";
    const int luaChon = NhapSoNguyen("Kieu tra cuu: ", 0, 2);
    const ParkingTicket* ve = NULL;
    if (luaChon == 0) {
        return;
    }
    if (luaChon == 1) {
        const int maVe = NhapSoNguyen("Ma ve: ", 1, std::numeric_limits<int>::max());
        ve = quanLy.TimVe(maVe);
    } else {
        const std::string bienSo = NhapDong("Bien so: ");
        ve = quanLy.TimXeDangGui(bienSo);
    }
    if (ve == NULL) {
        std::cout << "Khong tim thay ve phu hop.\n";
        return;
    }
    HienThiChiTietVe(*ve);
}

void HienThiThongKeLoai(const char* ten, const ThongKeLoaiXe& thongKe) {
    std::cout << std::left << std::setw(12) << ten << std::setw(12) << thongKe.TongO
              << std::setw(12) << thongKe.DangGui << std::setw(12) << thongKe.OTrong
              << DinhDangTyLe(thongKe.TyLeSuDung) << '\n';
}

void XemThongKe(const ParkingManager& quanLy) {
    const ThongKeBaiXe thongKe = quanLy.ThongKe();
    std::cout << "\n--- THONG KE VA DOANH THU ---\n"
              << "Tong o: " << thongKe.TongO << " | Dang gui: " << thongKe.DangGui
              << " | O trong: " << thongKe.OTrong
              << " | Su dung: " << DinhDangTyLe(thongKe.TyLeSuDung) << '\n'
              << std::left << std::setw(12) << "Loai xe" << std::setw(12) << "Tong o"
              << std::setw(12) << "Dang gui" << std::setw(12) << "O trong"
              << "Su dung\n" << std::string(60, '-') << '\n';
    HienThiThongKeLoai("Xe may", thongKe.XeMay);
    HienThiThongKeLoai("O to", thongKe.OTo);
    std::cout << "So luot da tra: " << thongKe.SoLuotDaTra
              << "\nDoanh thu da thu: " << thongKe.DoanhThu << " VND\n"
              << "Chinh sach hien tai: " << quanLy.LayChinhSachPhi().GetTen() << '\n';
    HienThiCanhBao(thongKe);
}

void DoiChinhSach(ParkingManager& quanLy, bool& daSuaDoi) {
    std::cout << "\n--- THAY DOI CHINH SACH TINH PHI ---\n"
              << "1. Theo gio: xe may 3000 VND/gio, o to 10000 VND/gio\n"
              << "   Lam tron len, toi thieu 1 gio (ke ca gui 0 phut).\n"
              << "2. Theo luot: xe may 8000 VND/luot, o to 25000 VND/luot\n"
              << "0. Giu nguyen\n"
              << "Chinh sach duoc chon tai luc tra xe, ke ca xe da vao truoc do.\n";
    const int luaChon = NhapSoNguyen("Lua chon: ", 0, 2);
    if (luaChon == 0) {
        return;
    }
    std::unique_ptr<FeePolicy> chinhSach;
    if (luaChon == 1) {
        chinhSach.reset(new HourlyFeePolicy());
    } else {
        chinhSach.reset(new FlatFeePolicy());
    }
    quanLy.DoiChinhSachPhi(std::move(chinhSach));
    daSuaDoi = true;
    std::cout << "Da doi chinh sach thanh: " << quanLy.LayChinhSachPhi().GetTen()
              << ". Cac ve tra tiep theo se dung chinh sach nay.\n";
}

void XuatDuLieu(const ParkingManager& quanLy) {
    std::cout << "\n--- XUAT FILE CSV / TXT ---\n"
              << "Xuat toan bo o do, ve dang gui, lich su va chinh sach phi.\n"
              << "File .csv va .txt cung dung dinh dang CSV cua chuong trinh.\n";
    const std::string duongDan = NhapDong("Duong dan file xuat (bo trong de huy): ");
    if (duongDan.empty()) {
        return;
    }
    quanLy.XuatFile(duongDan);
    std::cout << "Da xuat du lieu ra: " << duongDan << '\n';
}

void NhapDuLieu(ParkingManager& quanLy, bool& daSuaDoi) {
    std::cout << "\n--- NHAP FILE CSV / TXT ---\n"
              << "File hop le se THAY THE toan bo du lieu bai do hien tai.\n"
              << "File loi se bi tu choi va giu nguyen du lieu hien tai.\n";
    const std::string duongDan = NhapDong("Duong dan file nhap (bo trong de huy): ");
    if (duongDan.empty()) {
        return;
    }
    quanLy.NhapFile(duongDan);
    daSuaDoi = true;
    std::cout << "Da nhap va thay the du lieu tu: " << duongDan << '\n';
    HienThiCanhBao(quanLy.ThongKe());
}

std::string LayDuongDanChinh(int argc, char* argv[]) {
    if (argc > 2) {
        throw std::invalid_argument("Chi nhan toi da mot tham so: duong dan file du lieu chinh.");
    }
    if (argc == 2) {
        const std::string duongDan = argv[1];
        if (duongDan.empty()) {
            throw std::invalid_argument("Duong dan file du lieu chinh khong duoc rong.");
        }
#ifdef _WIN32
        const DWORD doDai = GetFullPathNameA(duongDan.c_str(), 0, NULL, NULL);
        if (doDai == 0) {
            throw std::runtime_error("Khong xac dinh duoc duong dan file du lieu chinh.");
        }
        std::vector<char> duongDanDayDu(doDai);
        const DWORD soKyTu = GetFullPathNameA(duongDan.c_str(), doDai,
                                             &duongDanDayDu[0], NULL);
        if (soKyTu == 0 || soKyTu >= doDai) {
            throw std::runtime_error("Khong xac dinh duoc duong dan file du lieu chinh.");
        }
        return std::string(&duongDanDayDu[0], soKyTu);
#else
        return duongDan;
#endif
    }

    std::string duongDanChuongTrinh;
#ifdef _WIN32
    std::vector<char> boDem(256);
    for (;;) {
        const DWORD soKyTu = GetModuleFileNameA(NULL, &boDem[0],
                                                static_cast<DWORD>(boDem.size()));
        if (soKyTu == 0) {
            throw std::runtime_error("Khong xac dinh duoc thu muc chua chuong trinh.");
        }
        if (soKyTu < boDem.size()) {
            duongDanChuongTrinh.assign(&boDem[0], soKyTu);
            break;
        }
        if (boDem.size() >= 32768) {
            throw std::runtime_error("Duong dan chuong trinh qua dai.");
        }
        boDem.resize(boDem.size() * 2);
    }
#else
    duongDanChuongTrinh = argc > 0 ? argv[0] : "";
#endif
    const std::string::size_type viTri = duongDanChuongTrinh.find_last_of("/\\");
    if (viTri == std::string::npos) {
        return "parking_data.csv";
    }
    return duongDanChuongTrinh.substr(0, viTri + 1) + "parking_data.csv";
}

bool FileDaTonTai(const std::string& duongDan) {
    errno = 0;
    std::ifstream file(duongDan.c_str(), std::ios::binary);
    if (file.is_open()) {
        return true;
    }
    if (errno == ENOENT) {
        return false;
    }
    throw std::runtime_error("Khong the mo file du lieu chinh: " + duongDan);
}

bool ThuLuuDuLieu(const ParkingManager& quanLy, const std::string& duongDan,
                  bool& chuaLuu) {
    if (!chuaLuu) {
        return true;
    }
    try {
        quanLy.XuatFile(duongDan);
        chuaLuu = false;
        std::cout << "Da tu dong luu du lieu vao file chinh.\n";
        return true;
    } catch (const std::exception& loi) {
        std::cout << "CANH BAO: Thao tac da thanh cong trong bo nho, nhung du lieu\n"
                  << "CHUA DUOC LUU vao file chinh: " << duongDan << '\n'
                  << "Loi luu file: " << loi.what() << '\n'
                  << "Co the chon muc 9 de xuat ban sao sang duong dan khac.\n"
                  << "Chuong trinh se thu luu lai sau thay doi tiep theo va khi thoat.\n";
        return false;
    }
}

int KetThucChuongTrinh(const ParkingManager& quanLy, const std::string& duongDan,
                       bool& chuaLuu, bool doEOF) {
    if (doEOF) {
        std::cout << "\nDa ket thuc luong nhap.\n";
    }
    if (!ThuLuuDuLieu(quanLy, duongDan, chuaLuu)) {
        std::cerr << "Thoat voi ma loi 1: con du lieu chua luu vao file chinh.\n";
        return 1;
    }
    std::cout << "Du lieu da duoc luu. Da thoat chuong trinh.\n";
    return 0;
}

void HienThiMenu(const ParkingManager& quanLy) {
    std::cout << "\n=============== QUAN LY BAI DO XE ===============\n"
              << "Chinh sach phi: " << quanLy.LayChinhSachPhi().GetTen() << '\n'
              << "1. Xem danh sach o do\n"
              << "2. Nhan xe vao bai\n"
              << "3. Tra xe va tinh phi\n"
              << "4. Tra cuu ve / bien so\n"
              << "5. Danh sach xe dang gui\n"
              << "6. Thong ke va doanh thu\n"
              << "7. Thay doi chinh sach tinh phi\n"
              << "8. Lich su ve da tra\n"
              << "9. Xuat file CSV / TXT\n"
              << "10. Nhap file CSV / TXT\n"
              << "0. Thoat\n"
              << "================================================\n";
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        const std::string duongDanChinh = LayDuongDanChinh(argc, argv);
        ParkingManager quanLy;
        std::cout << "HE THONG QUAN LY BAI DO XE - OOP C++11\n"
                  << "File du lieu chinh: " << duongDanChinh << '\n'
                  << "Tu dong nap khi mo va luu sau moi lan nhan/tra xe, doi phi, nhap file.\n"
                  << "Menu 9/10 xuat/nhap CSV hoac TXT; nhap file se thay the du lieu hien tai.\n"
                  << "Bang gia chi de mo phong.\n"
                  << "Thoi gian la so phut tu moc chung: 480 = 08:00 ngay 1,\n"
                  << "1500 = 01:00 ngay 2. Qua ngay van tiep tuc dem phut.\n"
                  << "Mac dinh theo gio: xe may 3000, o to 10000 VND/gio;\n"
                  << "lam tron len va thu toi thieu 1 gio.\n";
        if (FileDaTonTai(duongDanChinh)) {
            quanLy.NhapFile(duongDanChinh);
            std::cout << "Da nap du lieu tu file chinh.\n";
        } else {
            quanLy.XuatFile(duongDanChinh);
            std::cout << "Chua co file du lieu. Da tao file moi voi 6 o xe may va 4 o o to.\n";
        }
        bool chuaLuu = false;
        for (;;) {
            HienThiMenu(quanLy);
            bool daSuaDoi = false;
            try {
                const int luaChon = NhapSoNguyen("Nhap lua chon: ", 0, 10);
                switch (luaChon) {
                case 0:
                    return KetThucChuongTrinh(quanLy, duongDanChinh, chuaLuu, false);
                case 1:
                    XemDanhSachO(quanLy);
                    break;
                case 2:
                    NhanXe(quanLy, daSuaDoi);
                    break;
                case 3:
                    TraXe(quanLy, daSuaDoi);
                    break;
                case 4:
                    TraCuu(quanLy);
                    break;
                case 5:
                    std::cout << "\n--- DANH SACH XE DANG GUI ---\n";
                    HienThiDanhSachVe(quanLy.LayVeDangGui(), false);
                    break;
                case 6:
                    XemThongKe(quanLy);
                    break;
                case 7:
                    DoiChinhSach(quanLy, daSuaDoi);
                    break;
                case 8:
                    std::cout << "\n--- LICH SU VE DA TRA ---\n";
                    HienThiDanhSachVe(quanLy.LayLichSuVe(), true);
                    break;
                case 9:
                    XuatDuLieu(quanLy);
                    break;
                case 10:
                    NhapDuLieu(quanLy, daSuaDoi);
                    break;
                }
            } catch (const KetThucNhap&) {
                return KetThucChuongTrinh(quanLy, duongDanChinh, chuaLuu, true);
            } catch (const std::exception& loi) {
                std::cout << "Loi: " << loi.what() << '\n';
            }
            if (daSuaDoi) {
                chuaLuu = true;
                ThuLuuDuLieu(quanLy, duongDanChinh, chuaLuu);
            }
        }
    } catch (const std::exception& loi) {
        std::cerr << "Khong the khoi tao chuong trinh: " << loi.what() << '\n';
        return 1;
    }
}
