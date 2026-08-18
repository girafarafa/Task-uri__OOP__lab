# Gestiune Activități

Aplicație desktop în **C++17/20** cu interfață grafică **Qt** pentru gestionarea unei liste de activități (denumire, descriere, tip, durată). Proiect realizat ca temă de laborator, respectând principii de arhitectură software: **layered architecture** (Domain – Repository – Service – GUI), **Model-View** (Qt), **Observer**, **Undo/Command** și persistență în fișier.

## Cuprins

- [Funcționalități](#funcționalități)
- [Arhitectură](#arhitectură)
- [Structura proiectului](#structura-proiectului)
- [Cerințe](#cerințe)
- [Compilare și rulare](#compilare-și-rulare)
- [Testare](#testare)
- [Detalii de implementare](#detalii-de-implementare)
- [Limitări cunoscute / TODO](#limitări-cunoscute--todo)

## Funcționalități

**Fereastra principală**
- Adăugare, modificare, ștergere activități (CRUD complet), cu validare a datelor.
- Filtrare după tip și după descriere.
- Sortare după denumire, descriere sau durată (sortarea după durată grupează întâi pe tip).
- Raport agregat pe tipuri de activități (număr de activități + durată totală), afișat și ca butoane dinamice.
- **Undo** pentru ultimele operații (adăugare/ștergere/modificare), cu istoric vizibil.
- Persistență automată în fișier text (`activitati.txt`) — datele se încarcă la pornire și se salvează după fiecare modificare.

**Fereastra "Coș" (listă de lucru secundară)**
- Adăugare activități existente într-o listă separată de lucru ("coșul").
- Generare aleatoare de N activități din repository în coș.
- Export coș în format **CSV** și **HTML**.
- A doua fereastră, read-only, desenează grafic (cercuri) numărul curent de activități din coș și se actualizează automat prin pattern-ul **Observer** de fiecare dată când coșul se modifică.

## Arhitectură

```
        ┌───────────────┐
        │   MainWindow   │   ◄── GUI principal (CRUD, filtrare, sortare, raport, undo)
        │  ListaWindow   │   ◄── GUI secundar (coș: adaugă / generează / exportă)
        │ CosReadOnlyGUI │   ◄── GUI read-only, desenează coșul (Observer)
        └───────┬────────┘
                │ MyTableModel (QAbstractTableModel)
        ┌───────▼────────┐
        │    Service     │   ◄── logică de business, validare, undo, raport
        └───────┬────────┘
                │
        ┌───────▼────────┐
        │  Repo (interfață)│ ◄── appRepo (vector) / appRepoMap (unordered_map + eroare probabilistică)
        │    FileRepo      │ ◄── decorator peste appRepo, persistă în fișier text
        └───────┬────────┘
                │
        ┌───────▼────────┐
        │    Domain      │   ◄── clasa Activitate
        └────────────────┘
```

**Pattern-uri folosite:**
- **Repository** — interfața abstractă `Repo`, cu două implementări (`appRepo` pe `std::vector`, `appRepoMap` pe `std::unordered_map` cu simulare de eșec aleator) și un decorator `FileRepo` care adaugă persistență pe disc.
- **Service Layer** — clasa `service` centralizează logica (validare, undo, filtrare, sortare, raport) și expune un singur punct de intrare pentru GUI.
- **Observer** — `ListaActivitati` ("coșul") e `Observable`; `ListaWindow` și `CosReadOnlyGUI` sunt `Observer` și se actualizează automat la orice modificare a coșului.
- **Model-View (Qt)** — `MyTableModel : QAbstractTableModel` alimentează `QTableView`-urile, fără manipulare manuală rând-cu-rând.
- **Command / Undo** — `ActiuneUndo` (interfață) cu implementările `UndoAdauga`, `UndoSterge`, `UndoModifica`, stocate într-un istoric (`std::vector<std::unique_ptr<ActiuneUndo>>`).
- **Validare** — `ActivitateValidator` centralizează regulile de validitate pentru o `Activitate`, aruncând `ValidationException`.

## Structura proiectului

```
├── Domain.h / Domain.cpp          # Entitatea Activitate
├── repo.h / repo.cpp              # Repo (interfata), appRepo, appRepoMap, RepoException
├── FileRepo.h                     # Decorator peste appRepo cu persistenta in fisier text
├── validators.h                   # ActivitateValidator, ValidationException
├── undo.h                         # ActiuneUndo, UndoAdauga, UndoSterge, UndoModifica
├── raport.h                       # GenRaportDTO
├── Observer.h                     # Observer / Observable
├── lista_activitati.h / .cpp      # ListaActivitati ("cosul"), export CSV/HTML
├── service.h / service.cpp        # Stratul de business logic
├── MyTableModel.h                 # QAbstractTableModel pentru Model-View
├── MainWindow.h / .cpp            # Fereastra principala (CRUD, filtrare, sortare, raport, undo)
├── ListaWindow.h / .cpp           # Fereastra secundara pentru "cos"
├── CosReadOnlyGUI.h               # Fereastra read-only, desen grafic (Observer)
├── myvector.h                     # Container generic MyVector<T> (iteratori custom)
├── test.h / test.cpp              # Suita de teste (assert-based)
└── main.cpp                       # Punct de intrare (testAll() + pornire aplicatie Qt)
```

## Cerințe

- Compilator C++ cu suport **C++20** (folosește `std::ranges`, `std::construct_at`).
- **Qt 5** sau **Qt 6** (module: `Core`, `Gui`, `Widgets`).
- **CMake** ≥ 3.16 (sau Qt Creator, care detectează automat proiectul).

## Compilare și rulare

### Cu Qt Creator
1. Deschide `CMakeLists.txt` (sau fișierul `.pro`, după caz) în Qt Creator.
2. Configurează kit-ul (compilator + versiune Qt).
3. Build & Run.

### Din linia de comandă (CMake)

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=<calea_catre_Qt>
cmake --build .
./NumeExecutabil
```

> La pornire, `main.cpp` rulează automat suita de teste (`testAll()`) înainte de a deschide interfața grafică — dacă un test eșuează (`assert`), aplicația se oprește în consolă înainte de a arăta fereastra.

Datele activităților sunt persistate în `activitati.txt`, creat/citit automat de `FileRepo` din directorul curent de lucru.

## Testare

Testele sunt scrise cu `assert()` (fără framework extern) și acoperă:

- `Domain` — getteri/setteri.
- `Repo` (`appRepo`) — add / find / modifica / sterge / duplicate / inexistente.
- `Service` — CRUD, validare, filtrare, sortare, raport, undo (adaugă/șterge/modifică, undo multiplu, undo fără istoric).
- `ListaActivitati` (coș) — adăugare, golire, generare aleatoare, export CSV/HTML.
- `appRepoMap` — comportament cu probabilitate de eroare (0.0 și 1.0).
- `FileRepo` — persistență pe disc (creare, citire, modificare, ștergere, verificate prin re-instanțiere).

Rulare: testele pornesc automat la fiecare lansare a aplicației, din `main()`, prin `testAll()`.

## Detalii de implementare

- **Validare** (`validators.h`): o activitate e validă dacă denumirea, descrierea și tipul au minim 2 caractere, iar durata e strict pozitivă.
- **Undo**: fiecare operație de scriere (adaugă/șterge/modifică) își salvează inversul în istoric; `service::undo()` execută și elimină ultima acțiune.
- **appRepoMap**: implementare alternativă de repository ce simulează eșecuri aleatoare (utilă pentru testarea robusteței / gestionării excepțiilor), cu o probabilitate configurabilă la construcție.
- **Export**: `ListaActivitati::exportCSV/exportHTML` scriu coșul curent pe disc; apelate din GUI prin dialog de salvare (`QFileDialog`).

## Limitări cunoscute / TODO

- `MyVector<T>` (container generic cu iteratori proprii) este implementat, dar neintegrat momentan în restul aplicației (repository-urile folosesc `std::vector`) — util ca exercițiu separat de structuri de date.
- Testarea iteratorilor `MyVector` nu este încă acoperită.
- Poziționarea desenelor din `CosReadOnlyGUI` folosește `rand()` fără resetare explicită a seed-ului (`srand`); comportamentul aleator e reproductibil identic la fiecare rulare a aplicației.
