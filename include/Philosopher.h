#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include <semaphore.h>
#include <pthread.h>
#include <string>

enum class Mode {NAIVE, STARVATION, SAFE};

class Table;

class Philosopher {
public:
    Philosopher(int id, const std::string& name, sem_t* left, sem_t* right, int leftForkId, int rightForkId, sem_t* diningTable,
        pthread_barrier_t* startBarrier, Table* table, Mode mode = Mode::NAIVE); // sem_t* jako wskaźnik, bo nie możemy mieć kopii, zasoby mają być współdzielone
    void start();
    int getMealsEaten() const { return mealsEaten; }
    static void* threadEntry(void* arg);

private:
    int id;
    std::string name;
    pthread_t thread;
    sem_t* leftFork;
    sem_t* rightFork;
    int leftForkId;
    int rightForkId;
    pthread_barrier_t* startBarrier;
    sem_t* diningTable;
    Table* table;
    int mealsEaten = 0;
    Mode mode;

    void run();
    void run_naive();
    void run_starvation();
    void run_safe();
    void think();
    void eat();
    void hungry();
};

#endif
