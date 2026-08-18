#ifndef UNTITLED2_LISTA_ACTIVITATI_H
#define UNTITLED2_LISTA_ACTIVITATI_H

#include "Domain.h"
#include "Observer.h"
#include <vector>
#include <string>

// Acum lista mosteneste Observable
class ListaActivitati : public Observable {
    std::vector<Activitate> lista;
public:
    void golesteLista();
    void adaugaInLista(const Activitate& a);
    void genereazaAleator(int n, const std::vector<Activitate>& toateActivitatile);

    void exportCSV(const std::string& numeFisier) const;
    void exportHTML(const std::string& numeFisier) const;
    [[nodiscard]] const std::vector<Activitate>& getLista() const;
    [[nodiscard]] std::size_t size() const;
};

#endif //UNTITLED2_LISTA_ACTIVITATI_H