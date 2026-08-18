//
// Created by anast on 3/28/2026.
//

#include "Domain.h"
#include <iostream>
#include <utility>

Activitate::Activitate(const Activitate& ot)
    : denumire(ot.denumire), descriere(ot.descriere), tip(ot.tip), durata(ot.durata) {
    std::cout << "S-a creat o copie pentru:"<<denumire<<"\n";
}

Activitate::Activitate(string denumire, string descriere, string tip, double durata)
    : denumire{std::move(denumire)},
    descriere {std::move(descriere)},
    tip {std::move(tip)},
    durata{durata} {}

string Activitate::getDenumire() const {
    return this->denumire;
}

string Activitate::getDescriere() const {
    return this->descriere;
}

string Activitate::getTip() const {
    return this->tip;
}

double Activitate::getDurata() const noexcept {
    return this->durata;
}

void Activitate::setDurata(double durataNoua) noexcept {
    this->durata = durataNoua;
}

void Activitate::setTip(const string& tipNou) {
    this->tip = tipNou;
}

void Activitate::setDescriere(const string& descriereNoua) {
    this->descriere = descriereNoua;
}

void Activitate::setDenumire(const string& denumireNoua) {
    this->denumire = denumireNoua;
}
