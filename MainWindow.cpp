#include "MainWindow.h"
#include "ListaWindow.h"
#include "CosReadOnlyGUI.h"
#include <QTableWidget>
#include <QSplitter>
#include <QFont>
#include <QHeaderView>
#include <QMessageBox>

MainWindow::MainWindow(service& s, QWidget* parent)
    : QWidget(parent), serv(s) {
    setupUI();
    setupStyles();
    populateTabel(serv.getAll());
}

void MainWindow::setupUI() {
    setWindowTitle("Gestiune Activitati");
    setMinimumSize(1000, 650);

    auto* mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    // ── STANGA: tabel + filtrare + sortare ──
    auto* stangaLayout = new QVBoxLayout();

    // Bara de filtrare
    auto* filtruGroup = new QGroupBox("Filtrare si Sortare");
    auto* filtruLayout = new QHBoxLayout(filtruGroup);
    inputFiltruTip = new QLineEdit();
    inputFiltruTip->setPlaceholderText("Filtreaza dupa tip...");
    btnFiltruTip = new QPushButton("Filtru Tip");
    inputFiltruDescriere = new QLineEdit();
    inputFiltruDescriere->setPlaceholderText("Filtreaza dupa descriere...");
    btnFiltruDescriere = new QPushButton("Filtru Descriere");
    btnAfiseazaTot = new QPushButton("Afiseaza Tot");
    filtruLayout->addWidget(inputFiltruTip);
    filtruLayout->addWidget(btnFiltruTip);
    filtruLayout->addWidget(inputFiltruDescriere);
    filtruLayout->addWidget(btnFiltruDescriere);
    filtruLayout->addWidget(btnAfiseazaTot);

    // Bara de sortare
    auto* sortLayout = new QHBoxLayout();
    btnSortTitlu = new QPushButton("Dupa Titlu");
    btnSortDescriere = new QPushButton("Dupa Descriere");
    btnSortDurata = new QPushButton("Dupa Durata");
    sortLayout->addWidget(new QLabel("Sorteaza:"));
    sortLayout->addWidget(btnSortTitlu);
    sortLayout->addWidget(btnSortDescriere);
    sortLayout->addWidget(btnSortDurata);
    sortLayout->addStretch();

    // MODEL-VIEW: QTableView + MyTableModel
    tabelView = new QTableView();
    tabelModel = new MyTableModel(this);
    tabelView->setModel(tabelModel);
    tabelView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabelView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabelView->setSelectionMode(QAbstractItemView::SingleSelection);
    tabelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabelView->setAlternatingRowColors(true);

    // Tabel si lista widget side-by-side (cerinta lab 11)
    auto* viewLayout = new QHBoxLayout();
    viewLayout->addWidget(tabelView);
    listaWidget = new QListWidget();
    viewLayout->addWidget(listaWidget);

    stangaLayout->addWidget(filtruGroup);
    stangaLayout->addLayout(sortLayout);
    stangaLayout->addLayout(viewLayout);

    // ── DREAPTA: form + butoane ──
    auto* dreaptaLayout = new QVBoxLayout();
    dreaptaLayout->setSpacing(12);

    auto* formGroup = new QGroupBox("Date Activitate");
    auto* formLayout = new QVBoxLayout(formGroup);
    formLayout->setSpacing(8);
    auto addField = [&](const QString& label, QLineEdit*& field, const QString& placeholder) {
        formLayout->addWidget(new QLabel(label));
        field = new QLineEdit();
        field->setPlaceholderText(placeholder);
        formLayout->addWidget(field);
    };
    addField("Denumire:", inputDenumire, "ex: Fotbal");
    addField("Descriere:", inputDescriere, "ex: Meci amical");
    addField("Tip:", inputTip, "ex: Sport");
    addField("Durata (ore):", inputDurata, "ex: 1.5");

    // Butoane CRUD
    auto* crudGroup = new QGroupBox("Operatii");
    auto* crudLayout = new QVBoxLayout(crudGroup);
    btnAdauga   = new QPushButton("+ Adauga Activitate");
    btnModifica = new QPushButton("\u270E Modifica Selectia");
    btnSterge   = new QPushButton("\u2715 Sterge Selectia");
    btnUndo     = new QPushButton("Undo");
    for (auto* b : {btnAdauga, btnModifica, btnSterge, btnUndo})
        b->setMinimumHeight(36);
    crudLayout->addWidget(btnAdauga);
    crudLayout->addWidget(btnModifica);
    crudLayout->addWidget(btnSterge);
    crudLayout->addWidget(btnUndo);

    // Grup tipuri dinamice (cerinta lab 10)
    auto* tipuriGroup = new QGroupBox("Tipuri de Activitati");
    tipurilayout = new QVBoxLayout(tipuriGroup);
    tipurilayout->addWidget(new QLabel("Apasa un tip pentru detalii:"));

    // Butoane speciale
    btnRaport      = new QPushButton("Raport pe Tipuri");
    btnIstoric     = new QPushButton("Istoric Undo");
    btnCosCRUD     = new QPushButton("Cos CRUD \u2192");
    btnCosReadOnly = new QPushButton("Cos Vizualizare \u2192");
    for (auto* b : {btnRaport, btnIstoric, btnCosCRUD, btnCosReadOnly})
        b->setMinimumHeight(36);

    dreaptaLayout->addWidget(formGroup);
    dreaptaLayout->addWidget(crudGroup);
    dreaptaLayout->addWidget(tipuriGroup);
    dreaptaLayout->addStretch();
    dreaptaLayout->addWidget(btnRaport);
    dreaptaLayout->addWidget(btnIstoric);
    dreaptaLayout->addWidget(btnCosCRUD);
    dreaptaLayout->addWidget(btnCosReadOnly);

    mainLayout->addLayout(stangaLayout, 3);
    mainLayout->addLayout(dreaptaLayout, 1);

    // ── Conectare semnale ──
    connect(btnAdauga,          &QPushButton::clicked, this, &MainWindow::onAdauga);
    connect(btnSterge,          &QPushButton::clicked, this, &MainWindow::onSterge);
    connect(btnModifica,        &QPushButton::clicked, this, &MainWindow::onModifica);
    connect(btnFiltruTip,       &QPushButton::clicked, this, &MainWindow::onFiltruTip);
    connect(btnFiltruDescriere, &QPushButton::clicked, this, &MainWindow::onFiltruDescriere);
    connect(btnAfiseazaTot,     &QPushButton::clicked, this, &MainWindow::onAfiseazaTot);
    connect(btnSortTitlu,       &QPushButton::clicked, this, &MainWindow::onSortTitlu);
    connect(btnSortDescriere,   &QPushButton::clicked, this, &MainWindow::onSortDescriere);
    connect(btnSortDurata,      &QPushButton::clicked, this, &MainWindow::onSortDurata);
    connect(btnRaport,          &QPushButton::clicked, this, &MainWindow::onRaport);
    connect(btnIstoric,         &QPushButton::clicked, this, &MainWindow::onIstoric);
    connect(btnUndo,            &QPushButton::clicked, this, &MainWindow::onUndo);
    connect(btnCosCRUD,         &QPushButton::clicked, this, &MainWindow::onCosCRUD);
    connect(btnCosReadOnly,     &QPushButton::clicked, this, &MainWindow::onCosReadOnly);

    // Selectie din QTableView (nu QTableWidget)
    connect(tabelView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onSelectieSchimbata);

    genereazaButoaneTipuri();
}

// ── Cerinta lab 10: butoane dinamice per tip ──
void MainWindow::genereazaButoaneTipuri() {
    QLayoutItem* item;
    while ((item = tipurilayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    auto raport = serv.genereazaRaport();
    if (raport.empty()) {
        tipurilayout->addWidget(new QLabel("Nu exista tipuri."));
        return;
    }
    for (const auto& [tip, dto] : raport) {
        auto* btn = new QPushButton(QString::fromStdString(tip));
        btn->setStyleSheet(
            "QPushButton { background-color: #313244; border: 1px solid #45475a; "
            "border-radius: 6px; padding: 6px 14px; color: #cdd6f4; text-align: left; }"
            "QPushButton:hover { background-color: #45475a; border-color: #89b4fa; }");
        QString tipStr = QString::fromStdString(tip);
        int nr = dto.nrActivitati;
        connect(btn, &QPushButton::clicked, this, [this, tipStr, nr]() {
            QMessageBox::information(this, "Tip: " + tipStr,
                "Tipul: " + tipStr + "\nNumar activitati: " + QString::number(nr));
        });
        tipurilayout->addWidget(btn);
    }
}

void MainWindow::setupStyles() {
    setStyleSheet(R"(
        QWidget { background-color: #f8f9fa; color: #333333; font-family: 'Segoe UI', sans-serif; font-size: 13px; }
        QGroupBox { border: 1px solid #dcdcdc; border-radius: 8px; margin-top: 8px; padding-top: 8px; font-weight: bold; color: #6c757d; }
        QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }
        QLineEdit { background-color: #ffffff; border: 1px solid #ced4da; border-radius: 6px; padding: 6px 10px; color: #495057; }
        QLineEdit:focus { border: 1px solid #a3c2fa; background-color: #fdfdfe; }
        QTableView { background-color: #ffffff; border: 1px solid #e9ecef; border-radius: 8px; gridline-color: #f1f3f5; }
        QTableView::item { padding: 6px; }
        QTableView::item:selected { background-color: #d0ebff; color: #111111; }
        QTableView::item:alternate { background-color: #fbfbfc; }
        QHeaderView::section { background-color: #f1f3f5; color: #495057; padding: 8px; border: none; border-bottom: 1px solid #dee2e6; font-weight: bold; }
        QPushButton { background-color: #ffffff; border: 1px solid #ced4da; border-radius: 6px; padding: 6px 14px; color: #495057; }
        QPushButton:hover { background-color: #f8f9fa; border-color: #a3c2fa; }
        QPushButton:pressed { background-color: #e9ecef; }
        QLabel { color: #6c757d; font-size: 12px; }
    )");
    btnAdauga->setStyleSheet(
        "QPushButton { background-color: #dcedc8; color: #333333; font-weight: bold; border: 1px solid #c5e1a5; border-radius: 6px; padding: 6px 14px; }"
        "QPushButton:hover { background-color: #c5e1a5; }");
    btnSterge->setStyleSheet(
        "QPushButton { background-color: #ffcdd2; color: #333333; font-weight: bold; border: 1px solid #ef9a9a; border-radius: 6px; padding: 6px 14px; }"
        "QPushButton:hover { background-color: #ef9a9a; }");
    btnModifica->setStyleSheet(
        "QPushButton { background-color: #ffe0b2; color: #333333; font-weight: bold; border: 1px solid #ffcc80; border-radius: 6px; padding: 6px 14px; }"
        "QPushButton:hover { background-color: #ffcc80; }");
    btnUndo->setStyleSheet(
        "QPushButton { background-color: #f9e2af; color: #1e1e2e; font-weight: bold; border: none; border-radius: 6px; padding: 6px 14px; }"
        "QPushButton:hover { background-color: #e6c87a; }");
    btnCosCRUD->setStyleSheet(
        "QPushButton { background-color: #b3e5fc; color: #333333; font-weight: bold; border: 1px solid #81d4fa; border-radius: 6px; padding: 6px 14px; }"
        "QPushButton:hover { background-color: #81d4fa; }");
    btnCosReadOnly->setStyleSheet(
        "QPushButton { background-color: #e1bee7; color: #333333; font-weight: bold; border: 1px solid #ce93d8; border-radius: 6px; padding: 6px 14px; }"
        "QPushButton:hover { background-color: #ce93d8; }");
}

// ── MODEL-VIEW: populam modelul, nu randul cu randul ──
void MainWindow::populateTabel(const std::vector<Activitate>& activitati) {
    tabelModel->setActivitati(activitati);
    listaWidget->clear();
    for (const auto& a : activitati) {
        listaWidget->addItem(
            QString::fromStdString(a.getDenumire()) +
            " [" + QString::fromStdString(a.getTip()) + "]" +
            " - " + QString::number(a.getDurata()) + "h"
        );
    }
}

// ── Citim datele prin Model ──
void MainWindow::completezaCampuriDinSelectie() {
    auto rows = tabelView->selectionModel()->selectedRows();
    if (rows.isEmpty()) return;
    int row = rows.first().row();
    inputDenumire->setText(tabelModel->data(tabelModel->index(row, 0)).toString());
    inputDescriere->setText(tabelModel->data(tabelModel->index(row, 1)).toString());
    inputTip->setText(tabelModel->data(tabelModel->index(row, 2)).toString());
    inputDurata->setText(tabelModel->data(tabelModel->index(row, 3)).toString());
}

void MainWindow::onSelectieSchimbata() { completezaCampuriDinSelectie(); }

void MainWindow::onAdauga() {
    QString den  = inputDenumire->text().trimmed();
    QString desc = inputDescriere->text().trimmed();
    QString tip  = inputTip->text().trimmed();
    bool ok;
    double durata = inputDurata->text().trimmed().toDouble(&ok);
    if (!ok) { QMessageBox::warning(this, "Eroare", "Durata trebuie sa fie un numar!"); return; }
    try {
        serv.AddActivitate(den.toStdString(), desc.toStdString(), tip.toStdString(), durata);
        populateTabel(serv.getAll());
        inputDenumire->clear(); inputDescriere->clear(); inputTip->clear(); inputDurata->clear();
    } catch (ValidationException& e) {
        QMessageBox::warning(this, "Validare", QString::fromStdString(e.getErrorMessage()));
    } catch (RepoException& e) {
        QMessageBox::warning(this, "Eroare", QString::fromStdString(e.getErrorMessage()));
    }
    genereazaButoaneTipuri();
}

void MainWindow::onSterge() {
    auto rows = tabelView->selectionModel()->selectedRows();
    if (rows.isEmpty()) { QMessageBox::information(this, "Info", "Selecteaza o activitate!"); return; }
    int row = rows.first().row();
    QString den = tabelModel->data(tabelModel->index(row, 0)).toString();
    QString tip = tabelModel->data(tabelModel->index(row, 2)).toString();
    try {
        serv.stergeActivitate(den.toStdString(), tip.toStdString());
        populateTabel(serv.getAll());
        inputDenumire->clear(); inputDescriere->clear(); inputTip->clear(); inputDurata->clear();
    } catch (RepoException& e) {
        QMessageBox::warning(this, "Eroare", QString::fromStdString(e.getErrorMessage()));
    }
    genereazaButoaneTipuri();
}

void MainWindow::onModifica() {
    auto rows = tabelView->selectionModel()->selectedRows();
    if (rows.isEmpty()) { QMessageBox::information(this, "Info", "Selecteaza o activitate!"); return; }
    QString den  = inputDenumire->text().trimmed();
    QString desc = inputDescriere->text().trimmed();
    QString tip  = inputTip->text().trimmed();
    bool ok;
    double durata = inputDurata->text().trimmed().toDouble(&ok);
    if (!ok) { QMessageBox::warning(this, "Eroare", "Durata trebuie sa fie un numar!"); return; }
    try {
        serv.modificaActivitate(den.toStdString(), desc.toStdString(), tip.toStdString(), durata);
        populateTabel(serv.getAll());
    } catch (ValidationException& e) {
        QMessageBox::warning(this, "Validare", QString::fromStdString(e.getErrorMessage()));
    } catch (RepoException& e) {
        QMessageBox::warning(this, "Eroare", QString::fromStdString(e.getErrorMessage()));
    }
    genereazaButoaneTipuri();
}

void MainWindow::onFiltruTip() {
    QString tip = inputFiltruTip->text().trimmed();
    if (tip.isEmpty()) { onAfiseazaTot(); return; }
    populateTabel(serv.filtrareTip(tip.toStdString()));
}

void MainWindow::onFiltruDescriere() {
    QString desc = inputFiltruDescriere->text().trimmed();
    if (desc.isEmpty()) { onAfiseazaTot(); return; }
    populateTabel(serv.filtrareDescriere(desc.toStdString()));
}

void MainWindow::onAfiseazaTot() {
    inputFiltruTip->clear(); inputFiltruDescriere->clear();
    populateTabel(serv.getAll());
}

void MainWindow::onSortTitlu()     { populateTabel(serv.sortByTitlu()); }
void MainWindow::onSortDescriere() { populateTabel(serv.sortByDescriere()); }
void MainWindow::onSortDurata()    { populateTabel(serv.sortByDurata()); }

void MainWindow::onRaport() {
    auto raport = serv.genereazaRaport();
    if (raport.empty()) { QMessageBox::information(this, "Raport", "Nu exista activitati!"); return; }
    auto* w = new QWidget(nullptr, Qt::Window);
    w->setWindowTitle("Raport pe Tipuri"); w->setMinimumSize(400, 300); w->setStyleSheet(styleSheet());
    auto* layout = new QVBoxLayout(w);
    auto* tabelRaport = new QTableWidget();
    tabelRaport->setColumnCount(3);
    tabelRaport->setHorizontalHeaderLabels({"Tip", "Nr Activitati", "Durata Totala (ore)"});
    tabelRaport->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabelRaport->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (const auto& [tip, dto] : raport) {
        int row = tabelRaport->rowCount(); tabelRaport->insertRow(row);
        tabelRaport->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(tip)));
        tabelRaport->setItem(row, 1, new QTableWidgetItem(QString::number(dto.nrActivitati)));
        tabelRaport->setItem(row, 2, new QTableWidgetItem(QString::number(dto.durataTotala)));
    }
    layout->addWidget(tabelRaport); w->show();
}

//aici se deschid mai multe ferestre deodata
void MainWindow::onCosCRUD() {
    auto* fereastraNoua = new ListaWindow(serv);
    fereastraNoua->setAttribute(Qt::WA_DeleteOnClose);
    fereastraNoua->show();
}

void MainWindow::onCosReadOnly() {
    auto* fereastraNoua = new CosReadOnlyGUI(serv);
    fereastraNoua->setAttribute(Qt::WA_DeleteOnClose);
    fereastraNoua->show();
}

void MainWindow::onUndo() {
    try {
        serv.undo();
        populateTabel(serv.getAll());
        genereazaButoaneTipuri();
    } catch (RepoException& e) {
        QMessageBox::warning(this, "Undo", QString::fromStdString(e.getErrorMessage()));
    }
}

void MainWindow::onIstoric() {
    const auto& istoric = serv.getIstoric();
    if (istoric.empty()) { QMessageBox::information(this, "Istoric", "Nu exista operatii in istoric!"); return; }
    auto* w = new QWidget(nullptr, Qt::Window);
    w->setWindowTitle("Istoric Undo"); w->setMinimumSize(350, 300); w->setStyleSheet(styleSheet());
    auto* layout = new QVBoxLayout(w);
    auto* tabelIstoric = new QTableWidget();
    tabelIstoric->setColumnCount(1);
    tabelIstoric->setHorizontalHeaderLabels({"Operatie"});
    tabelIstoric->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabelIstoric->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (size_t i = 0; i < istoric.size(); ++i) {
        int row = tabelIstoric->rowCount(); tabelIstoric->insertRow(row);
        tabelIstoric->setItem(row, 0, new QTableWidgetItem(
            QString::number(i + 1) + ". " +
            QString::fromStdString(istoric.at(i)->getDescriere())));
    }
    layout->addWidget(tabelIstoric); w->show();
}