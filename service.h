#ifndef UNTITLED2_SERVICE_H
#define UNTITLED2_SERVICE_H

#include <unordered_map>
#include "Observer.h"
#include "repo.h"
#include "validators.h"
#include "raport.h"
#include "lista_activitati.h"
#include "undo.h"
#include <memory>

class service {
private:
    Repo* repo;
    ListaActivitati lista;
    std::vector<std::unique_ptr<ActiuneUndo>> istoricUndo ;

public:
    //explicit pentru a evita conversatiile implicite
    explicit service(Repo& repository) noexcept
           : repo{&repository} {};

    // Dezactivăm constructorul implicit (service are nevoie de repo și validator)
    service() = delete;

    /*
     * Adauga o aplicatie cu denumire, descriere, tip, durata
     * denumire: denumirea activitatii care se adauga(string)
     * descriere: descrierea activitatiii care se adauga(string)
     * tip: tipul activitatii care se adauga(string)
     * durata: durata care se adauga(double, format MM)
     *
     * throws:
     * RepoException daca mai exista o activitate cu denumirea si tipul dat
    *ValidationException daca activitatea nu este valida
    */
    void AddActivitate(const string& denumire,const string& descriere, const string& tip, double durata);


    /*
     * Sterge o activitate pe baza denumirii si a tipului
     */
    void stergeActivitate(const string& denumire,const string& tip);
    void modificaActivitate(const string& denumire,const string& descriere,const string& tip, double durata);

    void undo();

    //FILTRARI
    /*
     * Returneaza o lista cu toate activitatile dupa tipul dat
     *
     * tip: tipul dupa care se filtreaza
     * lista cu activitatile care au tipul dat
     */
    [[nodiscard]]std::vector<Activitate> filtrareTip(const string& tip)const ;

    /*
     * Returneaza o lista cu toate activitatile dupa descrierea data
     */
    [[nodiscard]]std::vector<Activitate> filtrareDescriere(const string& desc) const;


    //SORTARI
    [[nodiscard]]std::vector<Activitate> sortByTitlu() const;
    [[nodiscard]]std::vector<Activitate> sortByDurata() const;
    [[nodiscard]]std::vector<Activitate> sortByDescriere() const;

    /**
     *returneaza totate activitatile salvate in repo
     *return: o referinta constante catre vect de activitati
     *noexcept: promite ca nu arunca exceptii
    */
    [[nodiscard]] const std::vector<Activitate>& getAll() const noexcept {
        return repo->getActivitati();
    }

    /**
     * cauta o activitate dupa denumire si tip
     * @param denumire denumirea activitatii cautate
     * @param tip tipulactivitatii cautate
     * @return o referinta constanta catre activitate gasita
     * throws: repoexception daca activitatea nu a fost gasita
     */
    [[nodiscard]]const Activitate& cauta(const string& denumire,const string& tip) const;

    //METODE LISTA
    /**
     * sterge toate activitatile din lista curenta
     * post: lista de lucru devine goala
     */
    void golesteLista();

    /**
     * adauga o activitate din repository in lista de lucr
     * @param denumire denumirea activitatii (String)
     * @param tip tipul activitatii string
     * throws: repoException daca activitatea nu exista in repository
     * post: activitatea este adaugata in lista de lucru
     */
    void adaugaInLista(const std::string& denumire, const std::string& tip);

    /**
     * umple lista de lucru cu activitati alese aleatoriu din repository
     * @param n numarul de activitati de generat
     * post: lista de lucru contine n activitati noi
     */
    void genereazaAleator(int n);

    /**
     * Exporta actiivtatile din lista de lucru intr un fisier CSV
     * @param numeFisier calea catre fisierul de export
     * throws: exception daca fisierul nu poate fi deschis/scris
     */
    void exportCSV(const std::string& numeFisier) const;
    void exportHTML(const std::string& numeFisier) const;

    /**
     * Returneaza activitatile din lista de lucru curenta
     * @return o referinta constanta catre vect listei de lucru
     */
    [[nodiscard]] const std::vector<Activitate>& getLista() const;

    //nr de activitati din lista de lucru
    [[nodiscard]] std::size_t sizeLista() const;

    /**
     * Genereaza un raport bazat pe tipurile de activitati eistente in repo
     * @return un map unde cheia este tipul si valoarea este un obiect DTO
     * ce contine nr de activitati de acel tip
    */
    [[nodiscard]] std::unordered_map<std::string, GenRaportDTO> genereazaRaport() const;


    [[nodiscard]] const std::vector<std::unique_ptr<ActiuneUndo>>& getIstoric() const noexcept;

    // Adauga asta ca sa putem lega Observer-ul!
    ListaActivitati& getCosObj() { return lista; }

    void addObserverLista(Observer* obs) {
        lista.addObserver(obs);
    }

    void removeObserverLista(Observer* obs) {
        lista.removeObserver(obs);
    }
};


#endif //UNTITLED2_SERVICE_H