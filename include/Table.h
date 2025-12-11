#ifndef TABLE_H
#define TABLE_H

#include <atomic>
#include <vector>
#include <semaphore.h>
#include <mutex>
#include "Philosopher.h"


struct SharedState {
    std::vector<int> forkOwners;        // -1 = wolny, inaczej id filozofa
    std::vector<int> philosopherState;  // 1=THINK, 2=HUNGRY, 3=EAT
    std::vector<int> mealsEaten;        // liczba zjedzonych posiłków
    std::vector<std::string> names;     // imiona filozofów
    std::mutex mutex;                   // ochrona przy wizualizacji
};

class Table {
public:
    explicit Table(Mode mode = Mode::NAIVE);                   // konstruktor
    void initPhilosophers(Mode mode = Mode::NAIVE);   // tworzy filozofów i semafory
    void startDinner();        // uruchamia wszystkich
    SharedState shared;
private:
    static const int N = 5;    // liczba filozofów
    sem_t forks[N];            // tablica pałeczek (semaforów)
    pthread_barrier_t startBarrier;
    sem_t diningTable; // semafor stołu kelner
    std::atomic<int> nextPhilosopher = 0;
    std::vector<Philosopher> philosophers;  // lista filozofów

    friend class Philosopher;
};



#endif
