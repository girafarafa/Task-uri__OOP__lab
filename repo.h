#ifndef UNTITLED2_REPO_H
#define UNTITLED2_REPO_H

#include <unordered_map>

#include "Domain.h"
#include <vector>

using std::ostream;
using std::string;

/*
 * Clasa de exceptii specifice Repo
 */
class RepoException : public std::exception {
private:
    string errorMsg;
public:

    //constructor care initializeaza exceptia cu un mesaj specific de eroare
    explicit RepoException(string errorMsg) :errorMsg{std::move(errorMsg)} {};

    //returneaza mesajul de eroare stocat in exceptie
    [[nodiscard]] string getErrorMessage() const{
        return this->errorMsg;
    }

    //permite afisarea directa a mesajului de eroare al exceptiei
    friend ostream& operator<<(ostream& stream, const RepoException &exception) {
        stream<<exception.errorMsg;
        return stream;
    }
};


//CLASA ABSTRACTA
class Repo {
public: virtual void addActivitate(const Activitate& a)=0;
    virtual void stergeActivitatea(const string& titlu,const string& tip)=0;
    virtual void modificaActivitatea(const Activitate& a)=0;
    [[nodiscard]] virtual const Activitate& find(const string& titlu, const string & tip) const=0;

    [[nodiscard]] virtual bool isin(const Activitate& a) const = 0;
    [[nodiscard]] virtual const std::vector<Activitate>& getActivitati() const noexcept = 0;

    virtual ~Repo() = default;
};


//REPO CU VECTOR
class appRepo: public Repo {
private:
    std::vector<Activitate> allActivitati;

public:
    appRepo()=default;
    ~appRepo() = default;

    //constructor de copiere
    //punem delete pentru ca nu vrem sa se faca nicio copie
    //in aplicatie avem 1 singur Repo
    appRepo(const appRepo& ot)=delete;
    appRepo& operator=(const appRepo& ot) = delete;

    //permite mutarea obiectului in mod eficient
    appRepo(appRepo&& ot) noexcept = default;
    appRepo& operator=(appRepo&& ot) noexcept = default;

    /*
     * Adauga o activitate in lista
     * param a: activitatea care se adauga
     * return -
     * post: activitatea va fi adaugata in lista
     *
     * throws:RepoException daca o activitate cu acelasi titlu si tip exista deja
     */
    void addActivitate(const Activitate& a);

    /*
     * Returneaza o lista cu toate activitatile
     * return: lista cu activitatile ( vector of Activitate objects)
    */
    [[nodiscard]] const std::vector<Activitate>& getActivitati() const noexcept;

    /*
     * Cauta o activitate dupa titlu si tip dat
     *
     * titlu:titlu dupa care se cauta
     * tip: tipul dupa care se cauta
     * returns: entitatea Activitate cu titlu si tipul dat(daca aceasta exista)
     * throws: RepoException daca nu exista melodie cu titlul si tipul dat
     */
    [[nodiscard]] const Activitate& find(const string& titlu,const string& tip) const;

    /*
     * Verifica daca o activitate exista in lista
     * a: activitatea care se cauta in lista
     * return: true daca melodia exista, false altfel
     */
    [[nodiscard]] bool isin(const Activitate& a) const;


    //elimina din lista activitatea identificata prin combinatia unica de titlu si tip
    //arunca RepoException daca activitatea cautata nu se afla in lista
    void stergeActivitatea(const string& titlu,const string& tip);

    //actualizeaza datele unei activitati existente cu noile informatii furnizate
    //arunca RepoException daca nu exista nicio activitate care sa corespunda criteriilor pentru modificare
    void modificaActivitatea(const Activitate& a);
};


//REPO CU UNORDERED MAP + PROBABLITATE
class appRepoMap: public Repo {
private:
    std::unordered_map<string, Activitate> allActivitati;
    mutable std::vector<Activitate> cache;
    double probabilitate;

    //genereaza un nr random intre 0 si 1
    //daca e mai mic decat probabilitatea => arunca exceptie
    void arunca() const;
    void rebuiltCache() const;

public:
    explicit appRepoMap(double prob):probabilitate{prob} {}

    appRepoMap(const appRepoMap&) = delete;
    appRepoMap& operator=(const appRepoMap&) = delete;
    appRepoMap(appRepoMap&&) noexcept = default;
    appRepoMap& operator=(appRepoMap&&) noexcept = default;

    void addActivitate(const Activitate& a) override;
    void stergeActivitatea(const string& titlu, const string& tip) override;
    void modificaActivitatea(const Activitate& a) override;
    [[nodiscard]] const Activitate& find(const string& titlu, const string& tip) const override;
    [[nodiscard]] bool isin(const Activitate& a) const override;
    [[nodiscard]] const std::vector<Activitate>& getActivitati() const noexcept override;
};


#endif //UNTITLED2_REPO_H