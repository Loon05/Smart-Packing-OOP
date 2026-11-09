#include "ParkingTicket.h"

#include <limits>
#include <ostream>
#include <stdexcept>
#include <utility>

// Dung chung cho moi ve; khi nap file chi nang bo dem, khong giam lai.
int ParkingTicket::SoVeDaTao = 0;

ParkingTicket::ParkingTicket(std::unique_ptr<Vehicle> xe, int maO, int phutVao)
    : ParkingTicket(std::move(xe), maO, phutVao, 1, -1, 0, false) {
    if (SoVeDaTao == std::numeric_limits<int>::max()) {
        throw std::overflow_error("Da het ma ve co the cap.");
    }
    MaVe = ++SoVeDaTao;
}

ParkingTicket::ParkingTicket(std::unique_ptr<Vehicle> xe, int maO, int phutVao,
                             int maVe, int phutRa, long long phi, bool daKetThuc)
    : PhuongTien(std::move(xe)), MaVe(maVe), MaO(maO), PhutVao(phutVao),
      PhutRa(phutRa), Phi(phi), KetThuc(daKetThuc) {
    if (!PhuongTien) {
        throw std::invalid_argument("Ve phai gan voi mot phuong tien.");
    }
    if (maVe <= 0) {
        throw std::invalid_argument("Ma ve phai la so nguyen duong.");
    }
    if (maO <= 0) {
        throw std::invalid_argument("Ma o phai la so nguyen duong.");
    }
    if (phutVao < 0) {
        throw std::invalid_argument("Phut vao khong duoc am.");
    }
    if (daKetThuc) {
        if (phutRa < phutVao || phi < 0) {
            throw std::invalid_argument("Thoi gian hoac phi cua ve da tra khong hop le.");
        }
    } else if (phutRa != -1 || phi != 0) {
        throw std::invalid_argument("Ve dang gui phai co phut ra -1 va phi bang 0.");
    }
}

int ParkingTicket::GetSoVeDaTao() {
    return SoVeDaTao;
}

int ParkingTicket::GetMaVe() const {
    return MaVe;
}

int ParkingTicket::GetMaO() const {
    return MaO;
}

const Vehicle& ParkingTicket::GetXe() const {
    return *PhuongTien;
}

int ParkingTicket::GetPhutVao() const {
    return PhutVao;
}

int ParkingTicket::GetPhutRa() const {
    return PhutRa;
}

long long ParkingTicket::GetPhi() const {
    return Phi;
}

bool ParkingTicket::DaKetThuc() const {
    return KetThuc;
}

void ParkingTicket::DongVe(int phutRa, long long phi) {
    if (KetThuc) {
        throw std::logic_error("Ve da duoc tra, khong the tinh phi lan hai.");
    }
    if (phutRa < PhutVao) {
        throw std::invalid_argument("Phut ra phai lon hon hoac bang phut vao.");
    }
    if (phi < 0) {
        throw std::invalid_argument("Phi gui xe khong duoc am.");
    }
    // Chi cap nhat sau khi tat ca dieu kien deu hop le.
    PhutRa = phutRa;
    Phi = phi;
    KetThuc = true;
}

std::ostream& operator<<(std::ostream& out, const ParkingTicket& ve) {
    out << "Ve #" << ve.MaVe << " | " << TenLoaiXe(ve.PhuongTien->GetLoai())
        << " | Bien so: " << ve.PhuongTien->GetBienSo()
        << " | O: " << ve.MaO << "\n"
        << "Phut vao: " << ve.PhutVao;
    if (ve.KetThuc) {
        out << " | Phut ra: " << ve.PhutRa
            << " | Thoi gian gui: " << ve.PhutRa - ve.PhutVao << " phut\n"
            << "DA TRA | Phi: " << ve.Phi << " VND";
    } else {
        out << " | DANG GUI";
    }
    return out;
}
