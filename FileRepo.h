#ifndef UNTITLED2_FILEREPO_H
#define UNTITLED2_FILEREPO_H

#include "repo.h"
#include <fstream>
#include <sstream>

class FileRepo : public appRepo {
private:
    string numeFisier;

    void saveToFile() const {
        std::ofstream fout(numeFisier);
        for (const auto& a : getActivitati())
            fout << a.getDenumire() << ","
                 << a.getDescriere() << ","
                 << a.getTip() << ","
                 << a.getDurata() << "\n";
        fout.close();
    }

    void loadFromFile() {
        std::ifstream fin(numeFisier);
        string linie;
        while (std::getline(fin, linie)) {
            if (linie.empty()) continue;
            std::stringstream ss(linie);
            string denumire, descriere, tip, durataStr;
            std::getline(ss, denumire, ',');
            std::getline(ss, descriere, ',');
            std::getline(ss, tip, ',');
            std::getline(ss, durataStr, ',');
            double durata = std::stod(durataStr);
            appRepo::addActivitate({denumire, descriere, tip, durata});
        }
        fin.close();
    }

public:
    explicit FileRepo(string fisier) 
        : appRepo(), numeFisier{std::move(fisier)} {
        loadFromFile();
    }

    void addActivitate(const Activitate& a) override {
        appRepo::addActivitate(a);
        saveToFile();
    }

    void stergeActivitatea(const string& titlu, const string& tip) override {
        appRepo::stergeActivitatea(titlu, tip);
        saveToFile();
    }

    void modificaActivitatea(const Activitate& a) override {
        appRepo::modificaActivitatea(a);
        saveToFile();
    }
};

#endif