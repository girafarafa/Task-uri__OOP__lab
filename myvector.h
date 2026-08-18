#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <stdexcept>
#include <memory>
template <typename T>

class MyVector {
private:
    T* data;
    size_t capacity;
    size_t currentsize;
    //separa alocarea memoriei de constructia obiectelor
    std::allocator<T> alloc;

    //dubleaza capacitatea de stocare a vectorului atunci cand acesta devine plin
    void resize() {
        size_t newCapacity = (capacity == 0) ? 2 : capacity * 2;
        T* newData=alloc.allocate(newCapacity);
        for (size_t i=0; i<currentsize; i++) {
            std::construct_at(newData+i, std::move(data[i]));
            std::destroy_at(data+i);
        }
        alloc.deallocate(data, capacity);
        data=newData;
        capacity=newCapacity;
    }

public:
    using value_type = T;
    class Iterator {
    private:
        T* ptr;
    public:

        //constructor care initializeaza iteratorul cu adresa de memorie a unui element
        //explicit pentru a preveni conversiile implicite de la pointeri la obiecte
        explicit Iterator(T* p) noexcept : ptr(p) {}

        //operator de derefentiere
        T& operator*() { return *ptr; }

        //operator de acces la membrii obiectului indicat de iterator
        T* operator->() { return ptr; }

        //operator de incrementare (prefixat)
        Iterator& operator++() { ++ptr; return *this; }

        //verifica daca doi iteratori indica spre aceeasi adresa de memorie
        bool operator==(const Iterator& ot) const noexcept { return ptr == ot.ptr; }

        //verifica daca doi iteratori indica spre adrese diferite
        bool operator!=(const Iterator& ot) const noexcept { return ptr != ot.ptr; }
    };
    class ConstIterator {
    private:
        const T* ptr;
    public:

        //constructor care initializeaza iteratorul cu adresa unui element constant
        explicit ConstIterator(const T* p) noexcept : ptr(p) {}

        //operator de derefentiere pentru obiecte constante
        const T& operator*() const { return *ptr; }

        //operator de acces la membrii unui obiect constant
        const T* operator->() const { return ptr; }

        //avanseaza iteratorul la urm element
        ConstIterator& operator++() { ++ptr; return *this; }

        //compara egalitatea dintre doi iteratori de tip const
        bool operator==(const ConstIterator& ot) const noexcept { return ptr == ot.ptr; }

        //verifica inegalitatea intre doi iteratori de tip const
        bool operator!=(const ConstIterator& ot) const noexcept { return ptr != ot.ptr; }
    };


    //constructor implicit care initializeaza un vector gol cu o capacitate initiala de 2 elemente
    MyVector() : data{alloc.allocate(2)}, capacity{2}, currentsize{0} {}


    //destructorul clasei, distruge toate obiectele active si elibereaza intreaga memorie alocata
    ~MyVector() {
        for (size_t i=0; i<currentsize; i++)
            std::destroy_at(data+i);
        alloc.deallocate(data, capacity);
    }

    //copy constructor
    //creeaza o clona perfecta
    //c1 exista, se apeleaza ceva de genul activitate (c1)
    MyVector(const MyVector& ot) : data{alloc.allocate(ot.capacity)}, capacity{ot.capacity}, currentsize{ot.currentsize} {
        for (size_t i = 0; i < currentsize; i++)
            std::construct_at(data + i, ot.data[i]);
    }

    //copy assignment
    //actualizeaza un obiect care exista deja cu datele altuia
    MyVector& operator=(const MyVector& ot) {
        if (this==&ot)
            return *this;
        for (size_t i=0; i < currentsize; i++)
            std::destroy_at(data + i);
        alloc.deallocate(data, capacity);
        capacity = ot.capacity;
        currentsize = ot.currentsize;
        data = alloc.allocate(capacity);
        for (size_t i = 0; i < currentsize; i++)
            std::construct_at(data + i, ot.data[i]);
        return *this;
    }

    //Move constructor
    //std::move
    MyVector(MyVector&& ot) noexcept: data {ot.data}, capacity{ot.capacity}, currentsize{ot.currentsize} {
        ot.data=nullptr;
        ot.capacity=0;
        ot.currentsize=0;
    }

    //move assignemnt
    MyVector& operator=(MyVector&& ot) noexcept {
        if (this == &ot) return *this;
        for (size_t i = 0; i < currentsize; i++)
            std::destroy_at(data + i);
        alloc.deallocate(data, capacity);
        data=ot.data;
        capacity=ot.capacity;
        currentsize=ot.currentsize;
        ot.data=nullptr;
        ot.capacity=0;
        ot.currentsize=0;
        return *this;
    }

    //adauga un element nou la sfarsitul vectorului
    //daca dim a atins capac maxima, se apeleaza atuomat resize()
    void push_back(const T& elem) {
        if (currentsize==capacity)
            resize();
        std::construct_at(data + currentsize, elem);
        currentsize++;
    }

    //elimina elementul de la o pozitie specificata
    //distruge obiectul de la index si muta toate elem cu o poz spre stanga
    //arunca std::out_of_range daca indexul furnizat este mai mare sau egal cu o dim curenta
    void erase(size_t index) {
        if (index>=currentsize)
            throw std::out_of_range("index out of range");
        std::destroy_at(data + index);
        for (size_t i = index; i < currentsize - 1; i++) {
            std::construct_at(data + i, std::move(data[i + 1]));
            std::destroy_at(data + i + 1);
        }
        currentsize--;
    }

    //nr de elemente din vector
    [[nodiscard]] size_t size() const noexcept { return currentsize; }

    //verifica daca vect este gol, true daca este
    [[nodiscard]] bool empty() const noexcept { return currentsize==0; }

    //acces direct la elem de la pozitia index
    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }

    //ofera acces direct [] la elementul de la pozitia index
    T& at (size_t index) {
        if (index>=currentsize)
            throw std::out_of_range("index out of range");
        return data[index];
    }

    //ofera acces securizat () la elem de la poz index
    const T& at (size_t index) const {
        if (index>=currentsize)
            throw std::out_of_range("index out of range");
        return data[index];
    }

    //iteratori pt range-based for
    T*begin() noexcept { return data; }
    T*end() noexcept { return data+currentsize; }
    const T*begin() const noexcept { return data; }
    const T*end() const noexcept { return data+currentsize; }

};

#endif