#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTableView> // Inlocuit pt Model-View
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include "service.h"
#include <QListWidget>
#include "MyTableModel.h"

class ListaWindow;
class CosReadOnlyGUI; // Clasa noua de desen

class MainWindow : public QWidget {
    Q_OBJECT
private:
    service& serv;

    QTableView* tabelView; // VIEW
    MyTableModel* tabelModel; // MODEL

    QListWidget* listaWidget;
    QVBoxLayout* tipurilayout;
    void genereazaButoaneTipuri();

    QLineEdit* inputDenumire; QLineEdit* inputDescriere; QLineEdit* inputTip; QLineEdit* inputDurata;
    QLineEdit* inputFiltruTip; QLineEdit* inputFiltruDescriere;

    QPushButton* btnAdauga; QPushButton* btnSterge; QPushButton* btnModifica;
    QPushButton* btnFiltruTip; QPushButton* btnFiltruDescriere; QPushButton* btnAfiseazaTot;
    QPushButton* btnSortTitlu; QPushButton* btnSortDescriere; QPushButton* btnSortDurata;
    QPushButton* btnRaport;
    QPushButton* btnCosCRUD; // Buton pt prima fereastra (fostul ListaActivitati)
    QPushButton* btnCosReadOnly; // Buton pt a doua fereastra
    QPushButton* btnUndo; QPushButton* btnIstoric;

    void setupUI();
    void setupStyles();
    void populateTabel(const std::vector<Activitate>& activitati);
    void completezaCampuriDinSelectie();

public:
    explicit MainWindow(service& s, QWidget* parent = nullptr);

private slots:
    void onAdauga(); void onSterge(); void onModifica();
    void onFiltruTip(); void onFiltruDescriere(); void onAfiseazaTot();
    void onSortTitlu(); void onSortDescriere(); void onSortDurata();
    void onRaport();
    void onCosCRUD(); // Deschide fereastra 1
    void onCosReadOnly(); // Deschide fereastra 2
    void onSelectieSchimbata(); void onUndo(); void onIstoric();
};
#endif