// MyTableModel.h
#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "Domain.h"

class MyTableModel : public QAbstractTableModel {
private:
    std::vector<Activitate> activitati;

public:
    explicit MyTableModel(QObject* parent = nullptr) : QAbstractTableModel(parent) {}

    void setActivitati(const std::vector<Activitate>& noileActivitati) {
        beginResetModel();
        activitati = noileActivitati;
        endResetModel();
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return activitati.size();
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        return 4; // Denumire, Descriere, Tip, Durata
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid()) return QVariant();

        const auto& a = activitati[index.row()];

        if (role == Qt::DisplayRole) {
            switch (index.column()) {
                case 0: return QString::fromStdString(a.getDenumire());
                case 1: return QString::fromStdString(a.getDescriere());
                case 2: return QString::fromStdString(a.getTip());
                case 3: return QString::number(a.getDurata());
                default: return QVariant();
            }
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section) {
                case 0: return "Denumire";
                case 1: return "Descriere";
                case 2: return "Tip";
                case 3: return "Durata";
                default: return QVariant();
            }
        }
        return QVariant();
    }
};

#endif // MYTABLEMODEL_H