// CosReadOnlyGUI.h
#ifndef COSREADONLYGUI_H
#define COSREADONLYGUI_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <cstdlib>
#include "Observer.h"
#include "service.h"

class CosReadOnlyGUI : public QWidget, public Observer {
    Q_OBJECT
private:
    service& serv;
public:
    explicit CosReadOnlyGUI(service& s, QWidget* parent = nullptr) : QWidget(parent), serv(s) {
        // Ne abonam la modificarile cosului
        serv.getCosObj().addObserver(this);
        setWindowTitle("Cos Read-Only (Desene)");
        setMinimumSize(400, 400);
    }

    ~CosReadOnlyGUI() override {
        serv.getCosObj().removeObserver(this);
    }

    void update() override {
        //desene actualizate dupa fiecare modificare
        repaint();
    }

protected:
    void paintEvent(QPaintEvent* ev) override {
        QPainter p{this};
        // Luam dimensiunea curenta a listei
        int n = serv.getLista().size();

        for (int i = 0; i < n; ++i) {
            // Generam pozitii si marimi random
            int x = rand() % (width() - 40);
            int y = rand() % (height() - 40);
            int w = 20 + rand() % 40;

            p.drawEllipse(x, y, w, w); // Desenam un cerc pentru fiecare element
        }
    }
};

#endif // COSREADONLYGUI_H