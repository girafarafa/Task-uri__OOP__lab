#include "ListaWindow.h"
#include "CosReadOnlyGUI.h"
#include <QTableWidget>
#include <QSplitter>
#include <QFont>
#include <QHeaderView>
#include <QMessageBox>

ListaWindow::ListaWindow(service& s, QWidget* parent) : QWidget(parent), serv(s) {
    setupUI();
    setupStyles();

    //abonare la Observer
    serv.getCosObj().addObserver(this);

    refreshTabel();
}

ListaWindow::~ListaWindow() {
    //Dezabonare la inchidere
    serv.getCosObj().removeObserver(this);
}

//reactia la notificare
void ListaWindow::update() {
    refreshTabel();
}

void ListaWindow::setupUI() {
    setWindowTitle("Lista de Activitati (CosCRUDGUI)");
    setMinimumSize(600, 450);
    auto* mainLayout = new QVBoxLayout(this);

    // --- Arhitectura Model-View (Tabelul nou) ---
    tabelView = new QTableView();
    tabelModel = new MyTableModel(this);
    tabelView->setModel(tabelModel);
    tabelView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabelView->setSelectionBehavior(QAbstractItemView::SelectRows);

    labelNr = new QLabel("Numar activitati in lista: 0");

    auto* viewLayout = new QHBoxLayout();
    viewLayout->addWidget(tabelView);
    listaWidget = new QListWidget();
    viewLayout->addWidget(listaWidget);

    mainLayout->addLayout(viewLayout);
    mainLayout->addWidget(labelNr);

    // --- Sectiunea Adaugare Manuala ---
    auto* addGroup = new QGroupBox("Adauga activitate existenta");
    auto* addLayout = new QHBoxLayout(addGroup);
    inputDenumire = new QLineEdit(); inputDenumire->setPlaceholderText("Denumire...");
    inputTip = new QLineEdit(); inputTip->setPlaceholderText("Tip...");
    btnAdauga = new QPushButton("+ Adauga in Lista");
    addLayout->addWidget(inputDenumire); addLayout->addWidget(inputTip); addLayout->addWidget(btnAdauga);

    // --- Sectiunea Generare Aleatoare ---
    auto* genGroup = new QGroupBox("Generare Aleatoare");
    auto* genLayout = new QHBoxLayout(genGroup);
    inputNrAleator = new QSpinBox(); inputNrAleator->setMinimum(1); inputNrAleator->setMaximum(100);
    btnGenereaza = new QPushButton("Genereaza");
    genLayout->addWidget(new QLabel("Numar de activitati:"));
    genLayout->addWidget(inputNrAleator); genLayout->addWidget(btnGenereaza); genLayout->addStretch();

    // --- Sectiunea Actiuni ---
    auto* actLayout = new QHBoxLayout();
    btnGoleste = new QPushButton("Goleste Lista");
    btnExportCSV = new QPushButton("Export CSV");
    btnExportHTML = new QPushButton("Export HTML");
    actLayout->addWidget(btnGoleste); actLayout->addStretch();
    actLayout->addWidget(btnExportCSV); actLayout->addWidget(btnExportHTML);

    mainLayout->addWidget(addGroup); mainLayout->addWidget(genGroup); mainLayout->addLayout(actLayout);

    connect(btnAdauga, &QPushButton::clicked, this, &ListaWindow::onAdauga);
    connect(btnGoleste, &QPushButton::clicked, this, &ListaWindow::onGoleste);
    connect(btnGenereaza, &QPushButton::clicked, this, &ListaWindow::onGenereaza);
    connect(btnExportCSV, &QPushButton::clicked, this, &ListaWindow::onExportCSV);
    connect(btnExportHTML, &QPushButton::clicked, this, &ListaWindow::onExportHTML);
}

void ListaWindow::setupStyles() { /* Poti pastra stilurile tale vechi aici */ }

void ListaWindow::refreshTabel() {
    const auto& lista = serv.getLista();

    // Incarcam datele in model!
    tabelModel->setActivitati(lista);

    listaWidget->clear();
    for (const auto& a : lista) {
        QString text = QString::fromStdString(a.getDenumire()) + " [" + QString::fromStdString(a.getTip()) + "] - " + QString::number(a.getDurata()) + "h";
        listaWidget->addItem(text);
    }
    labelNr->setText("Numar activitati in lista: " + QString::number(lista.size()));
}

void ListaWindow::onAdauga() {
    QString den = inputDenumire->text().trimmed();
    QString tip = inputTip->text().trimmed();
    try {
        serv.adaugaInLista(den.toStdString(), tip.toStdString());
        inputDenumire->clear(); inputTip->clear();
    } catch (const std::exception& e) { QMessageBox::warning(this, "Eroare", e.what()); }
}
void ListaWindow::onGoleste() { serv.golesteLista(); } //automat
void ListaWindow::onGenereaza() { int n = inputNrAleator->value(); serv.genereazaAleator(n); } // automat

void ListaWindow::onExportCSV() {
    QString fisier = QFileDialog::getSaveFileName(this, "Export CSV", "", "CSV (*.csv)");
    if (fisier.isEmpty()) return;
    try {
        serv.getCosObj().exportCSV(fisier.toStdString()); // sau echivalent din service.h
        QMessageBox::information(this, "Export", "Export realizat cu succes!");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Eroare", e.what());
    }
}

void ListaWindow::onExportHTML() {
    if (serv.getLista().empty()) {
        QMessageBox::information(this, "Export HTML", "Lista este goala, nu ai ce exporta!");
        return;
    }

    QString fisier = QFileDialog::getSaveFileName(
        this, "Export HTML", "", "Fisiere HTML (*.html)");
    if (fisier.isEmpty()) return; // utilizatorul a anulat dialogul

    if (!fisier.endsWith(".html", Qt::CaseInsensitive))
        fisier += ".html";

    try {
        serv.getCosObj().exportHTML(fisier.toStdString());
        QMessageBox::information(this, "Export HTML", "Export realizat cu succes!");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Eroare", e.what());
    }
}

