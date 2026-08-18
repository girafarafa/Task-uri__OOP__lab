#include "lista_activitati.h"
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>

void ListaActivitati::golesteLista() {
    lista.clear();
    notify(); // ADĂUGAT
}

void ListaActivitati::adaugaInLista(const Activitate& a) {
    lista.push_back(a);
    notify(); // ADĂUGAT
}

void ListaActivitati::genereazaAleator(int n, const std::vector<Activitate>& all) {
    lista.clear();
    std::vector<Activitate> copie = all;
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::ranges::shuffle(copie, rng);

    int count = std::min(n, static_cast<int>(copie.size()));
    for (int i = 0; i < count; i++)
        lista.push_back(copie.at(i));

    notify(); // ADĂUGAT
}

void ListaActivitati::exportCSV(const std::string& numeFisier) const {
    std::ofstream f(numeFisier);
    if (!f.is_open()) throw std::runtime_error("Nu se poate deschide fisierul");
    f << "Denumire,Descriere,Tip,Durata\n";
    for (const auto& a : lista)
        f << a.getDenumire() << "," << a.getDescriere() << "," << a.getTip() << "," << a.getDurata() << "\n";
}

void ListaActivitati::exportHTML(const std::string& numeFisier) const {
    std::ofstream f(numeFisier);
    f << "<html><body><table border='1'>";
    f << "<tr><th>Denumire</th><th>Descriere</th><th>Tip</th><th>Durata</th></tr>";
    for (const auto& a : lista)
        f << "<tr><td>" << a.getDenumire() << "</td><td>" << a.getDescriere()
          << "</td><td>" << a.getTip() << "</td><td>" << a.getDurata() << "</td></tr>";
    f << "</table></body></html>";
}

const std::vector<Activitate>& ListaActivitati::getLista() const { return lista; }
std::size_t ListaActivitati::size() const { return lista.size(); }