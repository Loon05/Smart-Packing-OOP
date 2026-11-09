#ifndef PARKING_OOP_PARKING_FILE_STORAGE_H
#define PARKING_OOP_PARKING_FILE_STORAGE_H

#include <string>

class ParkingManager;

// Luu va phuc hoi mot anh chup du lieu; khong phu thuoc giao dien console.
class ParkingFileStorage {
public:
    static void Luu(const ParkingManager& quanLy, const std::string& duongDan);
    static void Nap(ParkingManager& quanLy, const std::string& duongDan);
};

#endif
