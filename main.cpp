#include "service.h"
#include "repo.h"
#include "FileRepo.h"
#include "test.h"
#include "MainWindow.h"
#include <QApplication> // Rezolva eroarea "Cannot resolve symbol 'QApplication'"


int main(int argc, char *argv[]) {
    testAll();

    //initializez aplicatia Qt
    QApplication app(argc, argv);

    //initializez repo si service
    FileRepo repo{"activitati.txt"};
    service serv{repo};

    //creez si afisez fereastra principala
    MainWindow w{serv};
    w.show();

    //event loop-ul Qt
    return app.exec();
}