#ifndef UNTITLED2_DOMAIN_H
#define UNTITLED2_DOMAIN_H
#include <string>
#include <iostream>
using std::string;

class Activitate {
private:
    string denumire;
    string descriere;
    string tip;
    double durata;
public:
    //interzice crearea unei activitati fara date initiale
    Activitate()=delete;

    //creeaza un obiect de tip Activitate cu datele furnizate
    //obiectul este initializat cu denumirea, descrierea, tipul si durata specifice
    Activitate(string denumire,string descriere, string tip,double durata);


    //destructorul implicit
    ~Activitate() = default;


    //constructor de copiere
    //creeaza o instanta noua prin copierea datelor dintr-un obiect existent
    Activitate(const Activitate& ot);
    Activitate& operator=(const Activitate& ot) = default;


    Activitate(Activitate&& ot) noexcept = default;
    Activitate& operator=(Activitate&& ot) noexcept = default;


    //verifica daca doua activitati sunt identice pe baza denumirii si tipului
    bool operator==(const Activitate& a)const {
        return this->getDenumire()==a.getDenumire() && this->getTip()==a.getTip();
    }

    //returneaza valorile atributelor private fara a le modifica
    [[nodiscard]] string getDenumire() const;
    [[nodiscard]] string getTip() const;
    [[nodiscard]] double getDurata() const noexcept;
    [[nodiscard]] string getDescriere() const;


    //seteaza valorile atributelor cu o valoare noua data
    void setDenumire(const string& d);
    void setTip(const string& t);
    void setDurata(double dura) noexcept;
    void setDescriere(const string& desc);

};



#endif //UNTITLED2_DOMAIN_H