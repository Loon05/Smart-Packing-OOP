#ifndef PARKING_OOP_PARKING_SLOT_H
#define PARKING_OOP_PARKING_SLOT_H

#include "Vehicle.h"

#include <iosfwd>

class ParkingSlot {
private:
    int MaO;
    VehicleType LoaiO;
    int KhoangCach;
    bool DangCoXe;

public:
    ParkingSlot(int maO, VehicleType loaiO, int khoangCach);

    int GetMaO() const;
    VehicleType GetLoaiO() const;
    int GetKhoangCach() const;
    bool CoXe() const;

    void ChiemO();
    void GiaiPhongO();
    bool operator<(const ParkingSlot& other) const;
    friend std::ostream& operator<<(std::ostream& out, const ParkingSlot& o);
};

#endif
