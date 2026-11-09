#ifndef PARKING_MANAGER_H
#define PARKING_MANAGER_H

#include "FeePolicy.h"
#include "ParkingSlot.h"
#include "ParkingTicket.h"

#include <memory>
#include <string>
#include <vector>

struct ThongKeLoaiXe {
    int TongO;
    int OTrong;
    int DangGui;
    double TyLeSuDung;
};

struct ThongKeBaiXe {
    int TongO;
    int OTrong;
    int DangGui;
    int SoLuotDaTra;
    double TyLeSuDung;
    long long DoanhThu;
    ThongKeLoaiXe XeMay;
    ThongKeLoaiXe OTo;
    bool CanhBaoGanDay;
};

class ParkingManager {
public:
    ParkingManager();

    const ParkingTicket& NhanXe(VehicleType loai, const std::string& bienSo,
                               int phutVao);
    const ParkingTicket& TraXe(int maVe, int phutRa);

    const ParkingTicket* TimVe(int maVe) const;
    const ParkingTicket* TimXeDangGui(const std::string& bienSo) const;
    const std::vector<ParkingSlot>& LayDanhSachO() const;
    std::vector<const ParkingTicket*> LayVeDangGui() const;
    std::vector<const ParkingTicket*> LayLichSuVe() const;

    void DoiChinhSachPhi(std::unique_ptr<FeePolicy> chinhSach);
    const FeePolicy& LayChinhSachPhi() const;
    ThongKeBaiXe ThongKe() const;

    void XuatFile(const std::string& duongDan) const;
    // Nap thanh cong se thay toan bo du lieu, huy cac con tro ve cu.
    void NhapFile(const std::string& duongDan);

private:
    friend class ParkingFileStorage;
    std::vector<ParkingSlot> DanhSachO;
    std::vector<std::unique_ptr<ParkingTicket> > VeDangGui;
    std::vector<std::unique_ptr<ParkingTicket> > LichSuVe;
    std::unique_ptr<FeePolicy> ChinhSachPhi;
    long long DoanhThu;

    ParkingSlot* ChonO(VehicleType loai);
    ParkingSlot* TimOTheoMa(int maO);
};

#endif
