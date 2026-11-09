#include "FeePolicy.h"

#include <stdexcept>

namespace {

void KiemTraSoPhut(int soPhut) {
    if (soPhut < 0) {
        throw std::invalid_argument("So phut gui xe khong duoc am.");
    }
}

long long DonGia(VehicleType loai, long long giaXeMay, long long giaOTo) {
    switch (loai) {
    case VehicleType::XeMay:
        return giaXeMay;
    case VehicleType::OTo:
        return giaOTo;
    }
    throw std::invalid_argument("Loai xe khong hop le.");
}

} // namespace

FeePolicy::~FeePolicy() {
}

long long HourlyFeePolicy::TinhPhi(VehicleType loai, int soPhut) const {
    KiemTraSoPhut(soPhut);
    const long long donGia = DonGia(loai, 3000LL, 10000LL);
    // Ep kieu truoc khi cong de van an toan khi soPhut bang INT_MAX.
    const long long soGioLamTron = (static_cast<long long>(soPhut) + 59LL) / 60LL;
    const long long soGioTinhPhi = soGioLamTron < 1LL ? 1LL : soGioLamTron;
    return soGioTinhPhi * donGia;
}

const char* HourlyFeePolicy::GetTen() const {
    return "Theo gio";
}

long long FlatFeePolicy::TinhPhi(VehicleType loai, int soPhut) const {
    KiemTraSoPhut(soPhut);
    return DonGia(loai, 8000LL, 25000LL);
}

const char* FlatFeePolicy::GetTen() const {
    return "Theo luot";
}
