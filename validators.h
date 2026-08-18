
#ifndef UNTITLED2_VALIDATORS_H
#define UNTITLED2_VALIDATORS_H

#include "Domain.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

class ValidationException : public std::exception {
    vector<string> errorMsg;

public:
    explicit ValidationException(vector<string> errorMessages)
        : errorMsg{std::move(errorMessages)}{};

    [[nodiscard]] string getErrorMessage() const{
        string fullMsg;
        for (const string& e: errorMsg) {
            fullMsg += e + "\n";
        }
        return fullMsg;
    }
};

/*
 * Clasa pentru validarea activitatilor
 * O activitate este valida daca:
 *  are titlu format din mai mult de 2 caractere
 *  are tip format din mai mult de 2 caractere
 *  durata activitatii este mai mare de 0 minute
 *  are descrierea formata din mai mult de 2 caractere
 *  formatul in care se da durata este MM
 */
class ActivitateValidator {

public:
    static void valideaza(const Activitate& a) {
        vector<string> errorMsg;
        if (a.getDenumire().length()<2)
            errorMsg.emplace_back("Titlul trebuie sa aiba cel putin 2 caractere. ");
        if (a.getDescriere().length()<2)
            errorMsg.emplace_back("Descrierea trebuie sa aiba cel putin 2 caractere");
        if (a.getTip().length()<2)
            errorMsg.emplace_back("Tipul trebuie sa aiba cel putin 2 caractere");
        if (a.getDurata()<=0)
            errorMsg.emplace_back("Durata trebuie sa fie un numar pozitiv! ");
        if (!errorMsg.empty())
            throw ValidationException(errorMsg);

    }
};

#endif