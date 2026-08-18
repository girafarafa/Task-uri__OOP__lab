#ifndef UNTITLED2_UNDO_H
#define UNTITLED2_UNDO_H
#include "repo.h"


class ActiuneUndo {
public:
    virtual void doUndo()=0;
    [[nodiscard]] virtual std::string getDescriere() const=0;
    virtual ~ActiuneUndo()=default;
};

class UndoAdauga : public ActiuneUndo {
private:
    Repo& repo;
    Activitate activitateAdaugata;
public:
    UndoAdauga(Repo& repo, const Activitate& a): repo{repo}, activitateAdaugata{a} {}

    void doUndo() override {
        repo.stergeActivitatea(activitateAdaugata.getDenumire(), activitateAdaugata.getTip());
    }
    [[nodiscard]] std::string getDescriere() const override {
        return "Adaugare: " + activitateAdaugata.getDenumire();
    }
};

class UndoSterge : public ActiuneUndo {
private:
    Repo& repo;
    Activitate activitateStersa;
public:
    UndoSterge(Repo& repo, const Activitate& a)
        : repo{repo}, activitateStersa{a} {}

    void doUndo() override {
        repo.addActivitate(activitateStersa);
    }
    [[nodiscard]] std::string getDescriere() const override {
        return "Stergere: " + activitateStersa.getDenumire();
    }

};

class UndoModifica: public ActiuneUndo {
    private:
    Repo& repo;
    Activitate activitateVeche;
    public:
        UndoModifica(Repo& repo, const Activitate& a): repo{repo}, activitateVeche{a} {}
    void doUndo() override {
            repo.modificaActivitatea(activitateVeche);
        }
    [[nodiscard]] std::string getDescriere() const override {
            return "Modificare: " + activitateVeche.getDenumire();
        }

};


#endif //UNTITLED2_UNDO_H