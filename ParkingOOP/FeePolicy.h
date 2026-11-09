#ifndef PARKING_OOP_FEE_POLICY_H
#define PARKING_OOP_FEE_POLICY_H

#include "Vehicle.h"

// Giao dien chung cho cac cach thu phi; ParkingManager chi can biet lop nay.
class FeePolicy {
public:
    virtual ~FeePolicy();

    virtual long long TinhPhi(VehicleType loai, int soPhut) const = 0;
    virtual const char* GetTen() const = 0;
};

class HourlyFeePolicy : public FeePolicy {
public:
    long long TinhPhi(VehicleType loai, int soPhut) const override;
    const char* GetTen() const override;
};

class FlatFeePolicy : public FeePolicy {
public:
    long long TinhPhi(VehicleType loai, int soPhut) const override;
    const char* GetTen() const override;
};

#endif
