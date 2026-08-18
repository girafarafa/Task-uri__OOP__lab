#include "test.h"
#include "Domain.h"
#include "repo.h"
#include "service.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include "FileRepo.h"
using std::string;

// --- DOMAIN ---
void testGetters() {
    Activitate a{"A", "B", "C", 10};
    assert(a.getDenumire() == "A");
    assert(a.getDescriere() == "B");
    assert(a.getTip() == "C");
    assert(a.getDurata() == 10);
}

void testSetters() {
    Activitate a{"A", "B", "C", 10};
    a.setDenumire("X");
    a.setDescriere("Y");
    a.setTip("Z");
    a.setDurata(20);
    assert(a.getDenumire() == "X");
    assert(a.getDescriere() == "Y");
    assert(a.getTip() == "Z");
    assert(a.getDurata() == 20);
}

// --- REPO ---
void testRepoAdd() {
    appRepo repo;
    Activitate a{"Titlu", "Descriere", "Tip", 10.5};
    repo.addActivitate(a);
    assert(!repo.getActivitati().empty());

    // duplicat => RepoException
    try {
        repo.addActivitate(a);
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testRepoCauta() {
    appRepo repo;
    repo.addActivitate({"AA", "BB", "CC", 10});
    [[maybe_unused]] auto& gasit = repo.find("AA", "CC");
    assert(gasit.getDescriere() == "BB");

    // inexistent => RepoException
    try {
        [[maybe_unused]] const auto& rez = repo.find("Inexistent", "Tip");
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testRepoModifica() {
    appRepo repo;
    repo.addActivitate({"AA", "BB", "CC", 10});

    Activitate modif{"AA", "Nou", "CC", 20};
    repo.modificaActivitatea(modif);
    assert(repo.find("AA", "CC").getDescriere() == "Nou");
    assert(repo.find("AA", "CC").getDurata() == 20);

    // inexistent => RepoException
    try {
        repo.modificaActivitatea({"XX", "YY", "ZZ", 5});
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testRepoSterge() {
    appRepo repo;
    repo.addActivitate({"AA", "BB", "CC", 10});
    repo.stergeActivitatea("AA", "CC");
    assert(repo.getActivitati().empty());

    // stergere inexistent => RepoException
    try {
        repo.stergeActivitatea("AA", "CC");
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

// --- SERVICE ---
void testServiceAdd() {
    appRepo repo;
    service serv{repo};

    serv.AddActivitate("AA", "BB", "CC", 10);
    assert(serv.getAll().size() == 1);

    // duplicat => RepoException
    try {
        serv.AddActivitate("AA", "BB", "CC", 10);
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }

    // denumire prea scurta => ValidationException
    try {
        serv.AddActivitate("A", "BB", "CC", 10);
        assert(false);
    } catch (const ValidationException&) {
        assert(true);
    }

    // durata negativa => ValidationException
    try {
        serv.AddActivitate("AA", "BB", "CC", -1);
        assert(false);
    } catch (const ValidationException&) {
        assert(true);
    }
}

void testServiceSterge() {
    appRepo repo;
    service serv{repo};

    serv.AddActivitate("AA", "BB", "CC", 10);
    serv.stergeActivitate("AA", "CC");
    assert(serv.getAll().empty());

    // stergere inexistent => RepoException
    try {
        serv.stergeActivitate("AA", "CC");
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testServiceModifica() {
    appRepo repo;
    service serv{repo};

    serv.AddActivitate("AA", "BB", "CC", 10);
    serv.modificaActivitate("AA", "DD", "CC", 20);

    [[maybe_unused]] const auto& a = serv.cauta("AA", "CC");
    assert(a.getDescriere() == "DD");
    assert(a.getDurata() == 20);

    // descriere prea scurta => ValidationException
    try {
        serv.modificaActivitate("AA", "D", "CC", 20);
        assert(false);
    } catch (const ValidationException&) {
        assert(true);
    }

    // inexistent => RepoException
    try {
        serv.modificaActivitate("XX", "YY", "ZZ", 10);
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testServiceCauta() {
    appRepo repo;
    service serv{repo};

    serv.AddActivitate("AA", "BB", "CC", 10);

    [[maybe_unused]] const auto& a = serv.cauta("AA", "CC");
    assert(a.getDenumire() == "AA");
    assert(a.getDescriere() == "BB");

    // inexistent => RepoException
    try {
        [[maybe_unused]] const auto& rez = serv.cauta("XX", "YY");
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testServiceFiltrareTip() {
    appRepo repo;
    service serv{repo};

    // lista goala
    auto rezEmpty = serv.filtrareTip("Sport");
    assert(rezEmpty.empty());

    serv.AddActivitate("Fotbal", "Meci", "Sport", 90);
    serv.AddActivitate("Mate", "Tema", "Scoala", 60);
    serv.AddActivitate("Alergare", "Parc", "Sport", 30);

    auto rez = serv.filtrareTip("Sport");
    assert(rez.size() == 2);

    // tip inexistent
    auto rez2 = serv.filtrareTip("Relaxare");
    assert(rez2.empty());
}

void testServiceFiltrareDescriere() {
    appRepo repo;
    service serv{repo};

    // lista goala
    auto rezEmpty = serv.filtrareDescriere("Meci");
    assert(rezEmpty.empty());

    serv.AddActivitate("Fotbal", "Meci", "Sport", 90);
    serv.AddActivitate("Tenis", "Meci", "Sport", 60);
    serv.AddActivitate("Mate", "Tema", "Scoala", 30);

    auto rez = serv.filtrareDescriere("Meci");
    assert(rez.size() == 2);

    // descriere inexistenta
    auto rez2 = serv.filtrareDescriere("Relaxare");
    assert(rez2.empty());
}

void testServiceSortTitlu() {
    appRepo repo;
    service serv{repo};

    // lista goala
    auto empty = serv.sortByTitlu();
    assert(empty.empty());

    serv.AddActivitate("Zumba", "Sala", "Sport", 60);
    serv.AddActivitate("Alergare", "Parc", "Sport", 30);
    serv.AddActivitate("Inot", "Piscina", "Sport", 45);

    auto rez = serv.sortByTitlu();
    assert(rez.size() == 3);
    assert(rez.at(0).getDenumire() == "Alergare");
    assert(rez.at(1).getDenumire() == "Inot");
    assert(rez.at(2).getDenumire() == "Zumba");
}

void testServiceSortDescriere() {
    appRepo repo;
    service serv{repo};

    // lista goala
    auto empty = serv.sortByDescriere();
    assert(empty.empty());

    serv.AddActivitate("Fotbal", "Stadion", "Sport", 90);
    serv.AddActivitate("Tenis", "Arena", "Sport", 60);
    serv.AddActivitate("Alergare", "Parc", "Sport", 30);

    auto rez = serv.sortByDescriere();
    assert(rez.size() == 3);
    assert(rez.at(0).getDescriere() == "Arena");
    assert(rez.at(1).getDescriere() == "Parc");
    assert(rez.at(2).getDescriere() == "Stadion");
}

void testServiceSortDurata() {
    appRepo repo;
    service serv{repo};

    // lista goala
    auto empty = serv.sortByDurata();
    assert(empty.empty());

    serv.AddActivitate("Fotbal", "Meci", "Sport", 90);
    serv.AddActivitate("Alergare", "Parc", "Sport", 30);
    serv.AddActivitate("Mate", "Tema", "Scoala", 60);

    auto rez = serv.sortByDurata();
    assert(rez.size() == 3);

    // verificam ca e sortat dupa tip, apoi dupa durata
    for (size_t i = 1; i < rez.size(); i++) {
        if (rez.at(i - 1).getTip() == rez.at(i).getTip()) {
            assert(rez.at(i - 1).getDurata() <= rez.at(i).getDurata());
        } else {
            assert(rez.at(i - 1).getTip() <= rez.at(i).getTip());
        }
    }
}

// --- LISTA ACTIVITATI ---
void testListaGoleste() {
    appRepo repo;
    service serv{repo};

    serv.AddActivitate("Fotbal", "Meci", "Sport", 90);
    serv.AddActivitate("Mate", "Tema", "Scoala", 60);

    serv.adaugaInLista("Fotbal", "Sport");
    serv.adaugaInLista("Mate", "Scoala");
    assert(serv.sizeLista() == 2);

    serv.golesteLista();
    assert(serv.sizeLista() == 0);
}

void testListaAdauga() {
    appRepo repo;
    service serv{repo};

    serv.AddActivitate("Fotbal", "Meci", "Sport", 90);
    serv.adaugaInLista("Fotbal", "Sport");
    assert(serv.sizeLista() == 1);
    assert(serv.getLista().at(0).getDenumire() == "Fotbal");

    // inexistent => RepoException
    try {
        serv.adaugaInLista("Inexistent", "Tip");
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testListaGenereazaAleator() {
    appRepo repo;
    service serv{repo};

    serv.AddActivitate("Fotbal", "Meci", "Sport", 90);
    serv.AddActivitate("Mate", "Tema", "Scoala", 60);
    serv.AddActivitate("Alergare", "Parc", "Sport", 30);

    // n mai mic decat total
    serv.genereazaAleator(2);
    assert(serv.sizeLista() == 2);

    // n mai mare decat total => se iau toate
    serv.genereazaAleator(100);
    assert(serv.sizeLista() == 3);

    // n = 0 => lista goala
    serv.genereazaAleator(0);
    assert(serv.sizeLista() == 0);

    // repo gol
    appRepo repoGol;
    service servGol{repoGol};
    servGol.genereazaAleator(5);
    assert(servGol.sizeLista() == 0);
}

void testListaExportCSV() {
    appRepo repo;
    service serv{repo};

    serv.AddActivitate("Fotbal", "Meci", "Sport", 90);
    serv.adaugaInLista("Fotbal", "Sport");

    // fisier valid
    serv.exportCSV("test_export.csv");
    std::ifstream f("test_export.csv");
    assert(f.is_open());
    string linie;
    std::getline(f, linie);
    assert(linie == "Denumire,Descriere,Tip,Durata");
    std::getline(f, linie);
    assert(linie.find("Fotbal") != string::npos);
    f.close();
}

void testListaExportHTML() {
    appRepo repo;
    service serv{repo};

    serv.AddActivitate("Fotbal", "Meci", "Sport", 90);
    serv.adaugaInLista("Fotbal", "Sport");

    serv.exportHTML("test_export.html");
    std::ifstream f("test_export.html");
    assert(f.is_open());
    string continut((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());
    assert(continut.find("Fotbal") != string::npos);
    assert(continut.find("<table") != string::npos);
    f.close();
}

void testRaport() {
    appRepo repo;
    service serv{repo};

    // raport gol
    auto raportGol = serv.genereazaRaport();
    assert(raportGol.empty());

    serv.AddActivitate("sport", "alergare", "sanatate", 1);
    serv.AddActivitate("psiholog", "terapie", "sanatate", 1);
    serv.AddActivitate("facultate", "oop", "educatie", 4);

    auto raport = serv.genereazaRaport();

    assert(raport.size() == 2);
    assert(raport["sanatate"].nrActivitati == 2);
    assert(raport["sanatate"].durataTotala == 2.0);
    assert(raport["educatie"].nrActivitati == 1);
    assert(raport["educatie"].durataTotala == 4.0);
}

void testUndo() {
    // undo adauga
    appRepo repo;
    service serv{repo};

    serv.AddActivitate("Fotbal", "Meci", "Sport", 90);
    assert(serv.getAll().size() == 1);
    serv.undo();
    assert(serv.getAll().empty());

    // undo sterge
    serv.AddActivitate("Fotbal", "Meci", "Sport", 90);
    serv.stergeActivitate("Fotbal", "Sport");
    assert(serv.getAll().empty());
    serv.undo();
    assert(serv.getAll().size() == 1);
    assert(serv.getAll().at(0).getDenumire() == "Fotbal");

    // undo modifica
    serv.modificaActivitate("Fotbal", "Antrenament", "Sport", 60);
    assert(serv.cauta("Fotbal", "Sport").getDescriere() == "Antrenament");
    serv.undo();
    assert(serv.cauta("Fotbal", "Sport").getDescriere() == "Meci");
    assert(serv.cauta("Fotbal", "Sport").getDurata() == 90);

    // undo multiplu
    serv.AddActivitate("Tenis", "Meci", "Sport", 60);
    serv.AddActivitate("Inot", "Piscina", "Sport", 45);
    assert(serv.getAll().size() == 3);
    serv.undo(); // anuleaza adauga Inot
    assert(serv.getAll().size() == 2);
    serv.undo(); // anuleaza adauga Tenis
    assert(serv.getAll().size() == 1);

    // undo fara operatii => RepoException
    appRepo repo2;
    service serv2{repo2};
    try {
        serv2.undo();
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

// --- REPO MAP ---

void testRepoMapProbabilitate() {
    // Setăm probabilitatea la 1.0 (100% șanse să pice)
    appRepoMap repo{1.0};
    Activitate a{"AA", "BB", "CC", 10};

    try {
        repo.addActivitate(a);
        assert(false); // Nu ar trebui să ajungă aici
    } catch (const RepoException&) {
        assert(true); // Excepția a fost aruncată corect de metoda arunca()
    }
}

void testRepoMapAdd() {
    appRepoMap repo{0.0}; // 0% șanse de eroare aleatoare
    Activitate a{"Titlu", "Descriere", "Tip", 10.5};

    repo.addActivitate(a);
    assert(repo.getActivitati().size() == 1);
    assert(repo.isin(a) == true);

    // Adăugare duplicat => RepoException
    try {
        repo.addActivitate(a);
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testRepoMapCauta() {
    appRepoMap repo{0.0};
    repo.addActivitate({"AA", "BB", "CC", 10});

    [[maybe_unused]] auto& gasit = repo.find("AA", "CC");
    assert(gasit.getDescriere() == "BB");

    // Căutare element inexistent => RepoException
    try {
        [[maybe_unused]] const auto& rez = repo.find("Inexistent", "Tip");
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testRepoMapModifica() {
    appRepoMap repo{0.0};
    repo.addActivitate({"AA", "BB", "CC", 10});

    Activitate modif{"AA", "Nou", "CC", 20};
    repo.modificaActivitatea(modif);

    assert(repo.find("AA", "CC").getDescriere() == "Nou");
    assert(repo.find("AA", "CC").getDurata() == 20);

    // Modificare element inexistent => RepoException
    try {
        repo.modificaActivitatea({"XX", "YY", "ZZ", 5});
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testRepoMapSterge() {
    appRepoMap repo{0.0};
    repo.addActivitate({"AA", "BB", "CC", 10});

    repo.stergeActivitatea("AA", "CC");
    assert(repo.getActivitati().empty());

    // Ștergere element inexistent => RepoException
    try {
        repo.stergeActivitatea("AA", "CC");
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testRepoMapIsIn() {
    appRepoMap repo{0.0};
    Activitate a{"AA", "BB", "CC", 10};
    Activitate b{"XX", "YY", "ZZ", 20};

    repo.addActivitate(a);

    assert(repo.isin(a) == true);
    assert(repo.isin(b) == false);
}

void testAllRepoMap() {
    testRepoMapProbabilitate();
    testRepoMapAdd();
    testRepoMapCauta();
    testRepoMapModifica();
    testRepoMapSterge();
    testRepoMapIsIn();
    std::cout << "Testele pentru appRepoMap au trecut cu succes!\n";
}
void testFileRepo() {
    // folosim un fisier de test separat
    FileRepo repo{"test_file_repo.csv"};

    // adauga
    repo.addActivitate({"Fotbal", "Meci", "Sport", 90});
    assert(repo.getActivitati().size() == 1);

    // persistenta - cream un nou repo din acelasi fisier
    FileRepo repo2{"test_file_repo.csv"};
    assert(repo2.getActivitati().size() == 1);
    assert(repo2.getActivitati().at(0).getDenumire() == "Fotbal");

    // modifica
    repo.modificaActivitatea({"Fotbal", "Antrenament", "Sport", 60});
    FileRepo repo3{"test_file_repo.csv"};
    assert(repo3.getActivitati().at(0).getDescriere() == "Antrenament");

    // sterge
    repo.stergeActivitatea("Fotbal", "Sport");
    FileRepo repo4{"test_file_repo.csv"};
    assert(repo4.getActivitati().empty());
}

void testRepoMap() {
    // probabilitate 0 => nu arunca niciodata exceptie
    appRepoMap repo{0.0};

    repo.addActivitate({"Fotbal", "Meci", "Sport", 90});
    assert(repo.getActivitati().size() == 1);

    // duplicat => RepoException
    try {
        repo.addActivitate({"Fotbal", "Meci", "Sport", 90});
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }

    // modifica
    repo.modificaActivitatea({"Fotbal", "Antrenament", "Sport", 60});
    assert(repo.find("Fotbal", "Sport").getDescriere() == "Antrenament");

    // sterge
    repo.stergeActivitatea("Fotbal", "Sport");
    assert(repo.getActivitati().empty());

    // sterge inexistent => RepoException
    try {
        repo.stergeActivitatea("Fotbal", "Sport");
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }

    // probabilitate 1 => arunca intotdeauna exceptie
    appRepoMap repoErori{1.0};
    try {
        repoErori.addActivitate({"Fotbal", "Meci", "Sport", 90});
        assert(false);
    } catch (const RepoException&) {
        assert(true);
    }
}

void testExportCSVEroare() {
    appRepo repo;
    service serv{repo};
    serv.AddActivitate("Fotbal", "Meci", "Sport", 90);
    serv.adaugaInLista("Fotbal", "Sport");

    // cale invalida => runtime_error
    try {
        serv.exportCSV("C:/cale/inexistenta/fisier.csv");
        assert(false);
    } catch (const std::runtime_error&) {
        assert(true);
    }
}

void testGetIstoric() {
    appRepo repo;
    service serv{repo};

    // istoric gol la inceput
    assert(serv.getIstoric().empty());

    serv.AddActivitate("Fotbal", "Meci", "Sport", 90);
    assert(serv.getIstoric().size() == 1);

    serv.stergeActivitate("Fotbal", "Sport");
    assert(serv.getIstoric().size() == 2);

    serv.undo();
    assert(serv.getIstoric().size() == 1);

    serv.undo();
    assert(serv.getIstoric().empty());
}

void testAll() {
    testGetters();
    testSetters();

    testRepoAdd();
    testRepoCauta();
    testRepoModifica();
    testRepoSterge();

    testServiceAdd();
    testServiceSterge();
    testServiceModifica();
    testServiceCauta();
    testServiceFiltrareTip();
    testServiceFiltrareDescriere();
    testServiceSortTitlu();
    testServiceSortDescriere();
    testServiceSortDurata();

    testListaAdauga();
    testListaExportCSV();
    testListaExportHTML();
    testListaGenereazaAleator();
    testListaGoleste();
    testRaport();
    testUndo();
    testExportCSVEroare();
    testGetIstoric();

    testAllRepoMap();
    testFileRepo();
    testRepoMap();
    std::cout << "Toate testele au trecut cu succes!\n";
}