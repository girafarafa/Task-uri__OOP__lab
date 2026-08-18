// Observer.h
#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>
#include <algorithm>

// Clasa pe care o vor mosteni ferestrele grafice
class Observer {
public:
    virtual void update() = 0;
    virtual ~Observer() = default;
};

// Clasa pe care o va mosteni ListaActivitati (Cosul)
class Observable {
private:
    std::vector<Observer*> observers;
public:
    void addObserver(Observer* obs) {
        observers.push_back(obs);
    }
    void removeObserver(Observer* obs) {
        observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
    }
protected:
    void notify() {
        for (auto obs : observers) {
            obs->update();
        }
    }
};

#endif // OBSERVER_H