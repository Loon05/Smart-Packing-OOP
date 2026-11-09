#include "ParkingManager.h"
#include "ParkingFileStorage.h"

#include <limits>
#include <stdexcept>
#include <utility>

ParkingManager::ParkingManager()
    : ChinhSachPhi(new HourlyFeePolicy()), DoanhThu(0) {
    DanhSachO.reserve(10);
    DanhSachO.push_back(ParkingSlot(101, VehicleType::XeMay, 20));
    DanhSachO.push_back(ParkingSlot(102, VehicleType::XeMay, 5));
    DanhSachO.push_back(ParkingSlot(103, VehicleType::XeMay, 12));
    DanhSachO.push_back(ParkingSlot(104, VehicleType::XeMay, 5));
    DanhSachO.push_back(ParkingSlot(105, VehicleType::XeMay, 30));
    DanhSachO.push_back(ParkingSlot(106, VehicleType::XeMay, 18));
    DanhSachO.push_back(ParkingSlot(201, VehicleType::OTo, 25));
    DanhSachO.push_back(ParkingSlot(202, VehicleType::OTo, 8));
    DanhSachO.push_back(ParkingSlot(203, VehicleType::OTo, 8));
    DanhSachO.push_back(ParkingSlot(204, VehicleType::OTo, 16));

    // So ve dang gui khong the vuot qua so o: cap phat truoc khi nhan xe.
    VeDangGui.reserve(DanhSachO.size());
}

ParkingSlot* ParkingManager::ChonO(VehicleType loai) {
    ParkingSlot* oTotNhat = NULL;
    for (std::size_t i = 0; i < DanhSachO.size(); ++i) {
        ParkingSlot& o = DanhSachO[i];
        if (!o.CoXe() && o.GetLoaiO() == loai &&
            (oTotNhat == NULL || o < *oTotNhat)) {
            oTotNhat = &o;
        }
    }
    return oTotNhat;
}

ParkingSlot* ParkingManager::TimOTheoMa(int maO) {
    for (std::size_t i = 0; i < DanhSachO.size(); ++i) {
        if (DanhSachO[i].GetMaO() == maO) {
            return &DanhSachO[i];
        }
    }
    return NULL;
}

const ParkingTicket& ParkingManager::NhanXe(VehicleType loai,
                                           const std::string& bienSo,
                                           int phutVao) {
    if (loai != VehicleType::XeMay && loai != VehicleType::OTo) {
        throw std::invalid_argument("Loai xe khong hop le.");
    }
    if (phutVao < 0) {
        throw std::invalid_argument("Phut vao phai la so nguyen khong am.");
    }
    const std::string bienSoChuan = Vehicle::ChuanHoaBienSo(bienSo);
    if (TimXeDangGui(bienSoChuan) != NULL) {
        throw std::invalid_argument("Bien so nay da co xe dang gui trong bai.");
    }

    ParkingSlot* o = ChonO(loai);
    if (o == NULL) {
        throw std::runtime_error("Da het o trong cho loai xe nay.");
    }

    std::unique_ptr<Vehicle> xe;
    if (loai == VehicleType::XeMay) {
        xe.reset(new Motorbike(bienSoChuan));
    } else {
        xe.reset(new Car(bienSoChuan));
    }
    std::unique_ptr<ParkingTicket> ve(
        new ParkingTicket(std::move(xe), o->GetMaO(), phutVao));

    // Sau khi tao ve thanh cong, cac thao tac duoi khong can cap phat bo nho.
    o->ChiemO();
    VeDangGui.push_back(std::move(ve));
    return *VeDangGui.back();
}

const ParkingTicket& ParkingManager::TraXe(int maVe, int phutRa) {
    std::size_t viTri = 0;
    while (viTri < VeDangGui.size() && VeDangGui[viTri]->GetMaVe() != maVe) {
        ++viTri;
    }
    if (viTri == VeDangGui.size()) {
        if (TimVe(maVe) != NULL) {
            throw std::invalid_argument("Ve nay da ket thuc, khong the tra xe lan nua.");
        }
        throw std::invalid_argument("Khong ton tai ma ve nay.");
    }

    ParkingTicket& ve = *VeDangGui[viTri];
    if (phutRa < ve.GetPhutVao()) {
        throw std::invalid_argument("Phut ra phai lon hon hoac bang phut vao.");
    }
    ParkingSlot* o = TimOTheoMa(ve.GetMaO());
    if (o == NULL || !o->CoXe()) {
        throw std::logic_error("Trang thai o do khong khop voi ve dang gui.");
    }

    const int soPhut = phutRa - ve.GetPhutVao();
    const long long phi = ChinhSachPhi->TinhPhi(ve.GetXe().GetLoai(), soPhut);
    if (phi < 0) {
        throw std::invalid_argument("Chinh sach tinh phi tra ve so tien am.");
    }
    if (phi > std::numeric_limits<long long>::max() - DoanhThu) {
        throw std::overflow_error("Doanh thu vuot gioi han luu tru.");
    }

    // Cap phat lich su truoc khi dong ve, de loi cap phat khong doi trang thai.
    LichSuVe.reserve(LichSuVe.size() + 1);
    ve.DongVe(phutRa, phi);
    o->GiaiPhongO();
    LichSuVe.push_back(std::move(VeDangGui[viTri]));
    VeDangGui.erase(VeDangGui.begin() + viTri);
    DoanhThu += phi;
    return *LichSuVe.back();
}

const ParkingTicket* ParkingManager::TimVe(int maVe) const {
    for (std::size_t i = 0; i < VeDangGui.size(); ++i) {
        if (VeDangGui[i]->GetMaVe() == maVe) {
            return VeDangGui[i].get();
        }
    }
    for (std::size_t i = 0; i < LichSuVe.size(); ++i) {
        if (LichSuVe[i]->GetMaVe() == maVe) {
            return LichSuVe[i].get();
        }
    }
    return NULL;
}

const ParkingTicket* ParkingManager::TimXeDangGui(const std::string& bienSo) const {
    const std::string bienSoChuan = Vehicle::ChuanHoaBienSo(bienSo);
    for (std::size_t i = 0; i < VeDangGui.size(); ++i) {
        if (VeDangGui[i]->GetXe().GetBienSo() == bienSoChuan) {
            return VeDangGui[i].get();
        }
    }
    return NULL;
}

const std::vector<ParkingSlot>& ParkingManager::LayDanhSachO() const {
    return DanhSachO;
}

std::vector<const ParkingTicket*> ParkingManager::LayVeDangGui() const {
    std::vector<const ParkingTicket*> ketQua;
    ketQua.reserve(VeDangGui.size());
    for (std::size_t i = 0; i < VeDangGui.size(); ++i) {
        ketQua.push_back(VeDangGui[i].get());
    }
    return ketQua;
}

std::vector<const ParkingTicket*> ParkingManager::LayLichSuVe() const {
    std::vector<const ParkingTicket*> ketQua;
    ketQua.reserve(LichSuVe.size());
    for (std::size_t i = 0; i < LichSuVe.size(); ++i) {
        ketQua.push_back(LichSuVe[i].get());
    }
    return ketQua;
}

void ParkingManager::DoiChinhSachPhi(std::unique_ptr<FeePolicy> chinhSach) {
    if (!chinhSach) {
        throw std::invalid_argument("Chinh sach tinh phi khong duoc rong.");
    }
    ChinhSachPhi = std::move(chinhSach);
}

const FeePolicy& ParkingManager::LayChinhSachPhi() const {
    return *ChinhSachPhi;
}

void ParkingManager::XuatFile(const std::string& duongDan) const {
    ParkingFileStorage::Luu(*this, duongDan);
}

void ParkingManager::NhapFile(const std::string& duongDan) {
    ParkingFileStorage::Nap(*this, duongDan);
}

ThongKeBaiXe ParkingManager::ThongKe() const {
    ThongKeBaiXe ketQua = {};
    ketQua.TongO = static_cast<int>(DanhSachO.size());
    ketQua.SoLuotDaTra = static_cast<int>(LichSuVe.size());
    ketQua.DoanhThu = DoanhThu;
    for (std::size_t i = 0; i < DanhSachO.size(); ++i) {
        const ParkingSlot& o = DanhSachO[i];
        ThongKeLoaiXe& theoLoai =
            (o.GetLoaiO() == VehicleType::XeMay) ? ketQua.XeMay : ketQua.OTo;
        ++theoLoai.TongO;
        if (o.CoXe()) {
            ++theoLoai.DangGui;
            ++ketQua.DangGui;
        } else {
            ++theoLoai.OTrong;
            ++ketQua.OTrong;
        }
    }
    if (ketQua.TongO != 0) {
        ketQua.TyLeSuDung = 100.0 * ketQua.DangGui / ketQua.TongO;
    }
    if (ketQua.XeMay.TongO != 0) {
        ketQua.XeMay.TyLeSuDung =
            100.0 * ketQua.XeMay.DangGui / ketQua.XeMay.TongO;
    }
    if (ketQua.OTo.TongO != 0) {
        ketQua.OTo.TyLeSuDung = 100.0 * ketQua.OTo.DangGui / ketQua.OTo.TongO;
    }
    ketQua.CanhBaoGanDay = ketQua.TyLeSuDung >= 80.0;
    return ketQua;
}
