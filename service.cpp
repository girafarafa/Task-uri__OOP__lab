//
// Created by anast on 3/28/2026.
//

#include "service.h"

#include <algorithm>
#include <iterator>
//IMPLEMENTARI NORMALE
void service::AddActivitate(const string& denumire, const string& descriere, const string& tip, double durata){
    Activitate a{denumire, descriere, tip, durata};
    ActivitateValidator::valideaza(a);
    repo->addActivitate(a);
    istoricUndo.push_back(std::make_unique<UndoAdauga>(*repo,a));
}

void service::stergeActivitate(const string& denumire,const string& tip) {
    const Activitate& a=repo->find(denumire,tip);
    istoricUndo.push_back(std::make_unique<UndoSterge>(*repo,a));
    repo->stergeActivitatea(denumire, tip);
}

void service::modificaActivitate(const string& denumire,const string& descriere,const string& tip, double durata) {
    const Activitate& veche = repo->find(denumire, tip);
    istoricUndo.push_back(std::make_unique<UndoModifica>(*repo, veche));
    Activitate noua{denumire, descriere, tip, durata};
    ActivitateValidator::valideaza(noua);
    repo->modificaActivitatea(noua);
}

void service::undo() {
    if (istoricUndo.empty())
        throw RepoException("Nu exista operatii de anulat.");
    istoricUndo.back()->doUndo();
    istoricUndo.pop_back();
}

const Activitate& service::cauta(const string& denumire, const string& tip)const {
    return repo->find(denumire, tip);
}

std::vector<Activitate> service::filtrareTip(const string& tip) const {
    const std::vector<Activitate> &all = repo->getActivitati();
    std::vector<Activitate> rez;
    std::ranges::copy_if(all.begin(), all.end(), std::back_inserter(rez),
    [&tip](const Activitate& a) { return a.getTip() == tip; });
    return rez;}



std::vector<Activitate> service::filtrareDescriere(const string& descriere)const {
    const std::vector<Activitate> &all = repo->getActivitati();
    std::vector<Activitate> rez;
    for (const auto &a: all) {
        if (a.getDescriere() == descriere) {
            rez.push_back(a);
        }
    }
    return rez;}

std::vector<Activitate> service::sortByTitlu() const {
    const std::vector<Activitate>& all = repo->getActivitati();
    std::vector<Activitate> v;
    for (const auto& a : all)
        v.push_back(a);

    std::ranges::sort(v, [](const Activitate& a1, const Activitate& a2) {
        return a1.getDenumire() < a2.getDenumire();
    });
    return v;}

std::vector<Activitate> service::sortByDescriere()const {
    const std::vector<Activitate>& all = repo->getActivitati();
    std::vector<Activitate> v;
    for (const auto& a : all)
        v.push_back(a);

    std::ranges::sort(v, [](const Activitate& a1, const Activitate& a2) {
        return a1.getDescriere() < a2.getDescriere();
    });
    return v;}

std::vector<Activitate> service::sortByDurata()const {
    const std::vector<Activitate>& all = repo->getActivitati();
    std::vector<Activitate> v;
    for (const auto& a : all)
        v.push_back(a);

    std::ranges::sort(v, [](const Activitate& a1, const Activitate& a2) {
        if (a1.getTip() == a2.getTip()) {
            return a1.getDurata() < a2.getDurata();
        }
        return a1.getTip() < a2.getTip();
    });
    return v;}


//IMPLEMENTARI LISTA
void service::golesteLista() {
    lista.golesteLista();
}

void service::adaugaInLista(const std::string& denumire, const std::string& tip) {
    const Activitate& a = repo->find(denumire, tip);  // arunca RepoException daca nu exista
    lista.adaugaInLista(a);
}

void service::genereazaAleator(int n) {
    lista.genereazaAleator(n, repo->getActivitati());
}

void service::exportCSV(const std::string& numeFisier) const {
    lista.exportCSV(numeFisier);
}

void service::exportHTML(const std::string& numeFisier) const {
    lista.exportHTML(numeFisier);
}

const std::vector<Activitate>& service::getLista() const {
    return lista.getLista();
}

std::size_t service::sizeLista() const {
    return lista.size();
}

std::unordered_map<std::string, GenRaportDTO> service::genereazaRaport() const {
    std::unordered_map<std::string, GenRaportDTO> raport;
    for (const auto& a : repo->getActivitati()) {
        raport[a.getTip()].nrActivitati++;
        raport[a.getTip()].durataTotala+=a.getDurata();
    }
    return raport;}

const std::vector<std::unique_ptr<ActiuneUndo>>& service::getIstoric() const noexcept {
    return istoricUndo;
}