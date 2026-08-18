#ifndef LISTAWINDOW_H
#define LISTAWINDOW_H

#include <QWidget>
#include <QTableView> // Modificat din QTableWidget
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include "service.h"
#include "Observer.h"
#include "MyTableModel.h"

// Acum mosteneste si Observer!
class ListaWindow : public QWidget, public Observer {
    Q_OBJECT
private:
    service& serv;

    QTableView* tabelView; //View
    MyTableModel* tabelModel; //Modelul meu

    QListWidget* listaWidget;
    QLabel* labelNr;
    QLineEdit* inputDenumire;
    QLineEdit* inputTip;
    QSpinBox* inputNrAleator;

    QPushButton* btnAdauga;
    QPushButton* btnGoleste;
    QPushButton* btnGenereaza;
    QPushButton* btnExportCSV;
    QPushButton* btnExportHTML;

    void setupUI();
    void setupStyles();
    void refreshTabel();

public:
    explicit ListaWindow(service& s, QWidget* parent = nullptr);
    ~ListaWindow() override; // Destructor pt dezabonare
    void update() override; // Functia chemata de Observer

private slots:
    void onAdauga();
    void onGoleste();
    void onGenereaza();
    void onExportCSV();
    void onExportHTML();
};
#endif