//
// Created by anast on 3/28/2026.
//

#include "repo.h"
#include <random>
#include <unordered_map>
#include <algorithm>

//arunca exceptie cu probabilitatea data
void appRepoMap::arunca() const {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    if (dist(rng) < probabilitate)
        throw RepoException("Operatie esuata aleator (probabilitate: "
                           + std::to_string(probabilitate) + ")");

}

//reconstruieste vectorul cache din map
void appRepoMap::rebuiltCache() const {
    cache.clear();
    for (const auto& [key, val] : allActivitati)
        cache.push_back(val);
}

void appRepoMap::addActivitate(const Activitate& a) {
    arunca();
    string key = a.getDenumire() + "_" + a.getTip();
    if (allActivitati.count(key))
        throw RepoException("Activitatea exista deja.");
    allActivitati.emplace(key, a);
    rebuiltCache();
}

void appRepoMap::stergeActivitatea(const string& titlu, const string& tip) {
    arunca();
    string key = titlu + "_" + tip;
    if (!allActivitati.count(key))
        throw RepoException("Activitatea nu exista.");
    allActivitati.erase(key);
    rebuiltCache();
}

void appRepoMap::modificaActivitatea(const Activitate& a) {
    arunca();
    string key = a.getDenumire() + "_" + a.getTip();
    if (!allActivitati.count(key))
        throw RepoException("Activitatea pentru modificare nu a fost gasita.");
    allActivitati.at(key) = a;
    rebuiltCache();
}

const Activitate& appRepoMap::find(const string& titlu, const string& tip) const {
    arunca();
    string key = titlu + "_" + tip;
    auto it = allActivitati.find(key);
    if (it == allActivitati.end())
        throw RepoException("Activitatea cu titlul " + titlu + " si tipul " + tip + " nu exista in lista");
    return it->second;
}

bool appRepoMap::isin(const Activitate& a) const {
    arunca();
    string key = a.getDenumire() + "_" + a.getTip();
    return allActivitati.count(key) > 0;
}

const std::vector<Activitate>& appRepoMap::getActivitati() const noexcept {
    rebuiltCache();
    return cache;
}



bool appRepo::isin(const Activitate& a) const{
    return std::ranges::any_of(allActivitati, [&](const auto& activitateInRepo) {
        return activitateInRepo == a;
    });
}


const Activitate& appRepo::find(const string& titlu,const string& tip)const {
    auto it=std::ranges::find_if(allActivitati.begin(), allActivitati.end(),[&](const Activitate& a) {
        return a.getDenumire() == titlu && a.getTip() == tip;
    });
    if ( it==allActivitati.end() )
        throw RepoException("Activitatea cu titlul " + titlu + " si tipul " + tip + " nu exista in lista");
    return *it;
}

void appRepo::addActivitate(const Activitate &a) {
    if (isin(a))
        throw RepoException("Activitatea cu titlul "+a.getDenumire()+" si tipul"+a.getTip()+" exista deja in lista");
    this->allActivitati.push_back(a);
}

const std::vector<Activitate> &appRepo::getActivitati() const noexcept {
    return this->allActivitati;
}

void appRepo::stergeActivitatea(const string& titlu, const string& tip) {
    auto it = std::ranges::find_if(allActivitati.begin(), allActivitati.end(),
        [&](const Activitate& a) {
            return a.getDenumire() == titlu && a.getTip() == tip;
        });

    if (it == allActivitati.end())
        throw RepoException("Activitatea nu exista.");

    allActivitati.erase(it);
}

void appRepo::modificaActivitatea(const Activitate &a) {
    auto it = std::ranges::find_if(allActivitati.begin(), allActivitati.end(),
        [&a](const Activitate& activitate) {
            return activitate.getDenumire() == a.getDenumire()
                && activitate.getTip() == a.getTip();
        });

    if (it == allActivitati.end())
        throw RepoException("Activitatea pentru modificare nu a fost gasita.");

    *it = a;
}

