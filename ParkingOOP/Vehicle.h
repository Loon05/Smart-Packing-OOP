#ifndef PARKING_OOP_VEHICLE_H
#define PARKING_OOP_VEHICLE_H

#include <string>

enum class VehicleType { XeMay, OTo };

const char* TenLoaiXe(VehicleType loai);

// Giao dien chung cho cac phuong tien; khong tao Vehicle truc tiep.
class Vehicle {
private:
    std::string BienSo;

public:
    explicit Vehicle(const std::string& bienSo);
    virtual ~Vehicle();

    const std::string& GetBienSo() const;
    virtual VehicleType GetLoai() const = 0;
    static std::string ChuanHoaBienSo(const std::string& bienSo);
};

class Motorbike : public Vehicle {
public:
    explicit Motorbike(const std::string& bienSo);
    VehicleType GetLoai() const override;
};

class Car : public Vehicle {
public:
    explicit Car(const std::string& bienSo);
    VehicleType GetLoai() const override;
};

#endif
