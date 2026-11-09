#ifndef PARKING_OOP_PARKING_TICKET_H
#define PARKING_OOP_PARKING_TICKET_H

#include "Vehicle.h"

#include <iosfwd>
#include <memory>

class ParkingTicket {
private:
    // Chi lop luu tru duoc khoi phuc ma ve cu, khong cap lai ma moi.
    friend class ParkingFileStorage;
    static int SoVeDaTao;
    std::unique_ptr<Vehicle> PhuongTien;
    int MaVe;
    int MaO;
    int PhutVao;
    int PhutRa;
    long long Phi;
    bool KetThuc;

    ParkingTicket(std::unique_ptr<Vehicle> xe, int maO, int phutVao,
                  int maVe, int phutRa, long long phi, bool daKetThuc);

public:
    ParkingTicket(std::unique_ptr<Vehicle> xe, int maO, int phutVao);
    // Mot luot gui chi co mot ve; cam sao chep de khong trung ma ve.
    ParkingTicket(const ParkingTicket&) = delete;
    ParkingTicket& operator=(const ParkingTicket&) = delete;

    static int GetSoVeDaTao();
    int GetMaVe() const;
    int GetMaO() const;
    const Vehicle& GetXe() const;
    int GetPhutVao() const;
    int GetPhutRa() const;
    long long GetPhi() const;
    bool DaKetThuc() const;

    void DongVe(int phutRa, long long phi);
    friend std::ostream& operator<<(std::ostream& out, const ParkingTicket& ve);
};

#endif
