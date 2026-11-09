#include "Vehicle.h"

#include <cctype>
#include <stdexcept>

const char* TenLoaiXe(VehicleType loai) {
    switch (loai) {
    case VehicleType::XeMay:
        return "Xe may";
    case VehicleType::OTo:
        return "O to";
    }
    throw std::invalid_argument("Loai xe khong hop le.");
}

Vehicle::Vehicle(const std::string& bienSo)
    : BienSo(ChuanHoaBienSo(bienSo)) {
}

Vehicle::~Vehicle() {
}

const std::string& Vehicle::GetBienSo() const {
    return BienSo;
}

std::string Vehicle::ChuanHoaBienSo(const std::string& bienSo) {
    std::string ketQua;
    for (std::string::const_iterator it = bienSo.begin(); it != bienSo.end(); ++it) {
        const unsigned char kyTu = static_cast<unsigned char>(*it);
        // Bo khoang trang va viet hoa de tranh trung bien so do cach nhap.
        if (std::isspace(kyTu)) {
            continue;
        }
        if (std::iscntrl(kyTu)) {
            throw std::invalid_argument("Bien so chua ky tu dieu khien khong hop le.");
        }
        ketQua += static_cast<char>(std::toupper(kyTu));
    }
    if (ketQua.empty()) {
        throw std::invalid_argument("Bien so khong duoc de trong.");
    }
    return ketQua;
}

Motorbike::Motorbike(const std::string& bienSo) : Vehicle(bienSo) {
}

VehicleType Motorbike::GetLoai() const {
    return VehicleType::XeMay;
}

Car::Car(const std::string& bienSo) : Vehicle(bienSo) {
}

VehicleType Car::GetLoai() const {
    return VehicleType::OTo;
}
