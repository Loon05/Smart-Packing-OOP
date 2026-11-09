#include "ParkingSlot.h"

#include <ostream>
#include <stdexcept>

ParkingSlot::ParkingSlot(int maO, VehicleType loaiO, int khoangCach)
    : MaO(maO), LoaiO(loaiO), KhoangCach(khoangCach), DangCoXe(false) {
    if (maO <= 0) {
        throw std::invalid_argument("Ma o phai la so nguyen duong.");
    }
    if (khoangCach < 0) {
        throw std::invalid_argument("Khoang cach den cong khong duoc am.");
    }
    TenLoaiXe(loaiO);
}

int ParkingSlot::GetMaO() const {
    return MaO;
}

VehicleType ParkingSlot::GetLoaiO() const {
    return LoaiO;
}

int ParkingSlot::GetKhoangCach() const {
    return KhoangCach;
}

bool ParkingSlot::CoXe() const {
    return DangCoXe;
}

void ParkingSlot::ChiemO() {
    if (DangCoXe) {
        throw std::logic_error("O do da co xe.");
    }
    DangCoXe = true;
}

void ParkingSlot::GiaiPhongO() {
    if (!DangCoXe) {
        throw std::logic_error("O do da trong.");
    }
    DangCoXe = false;
}

bool ParkingSlot::operator<(const ParkingSlot& other) const {
    if (KhoangCach != other.KhoangCach) {
        return KhoangCach < other.KhoangCach;
    }
    return MaO < other.MaO;
}

std::ostream& operator<<(std::ostream& out, const ParkingSlot& o) {
    return out << "O " << o.MaO << " | " << TenLoaiXe(o.LoaiO)
               << " | Cach cong: " << o.KhoangCach << " m | "
               << (o.DangCoXe ? "DA CO XE" : "TRONG");
}
