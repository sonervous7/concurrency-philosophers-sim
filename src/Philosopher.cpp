//
// Created by sonervous7 on 30.10.2025.
//

#include "../include/Philosopher.h"
#include "Table.h"
#include <iostream>
#include <unistd.h>



using namespace std;

Philosopher::Philosopher(int id, const std::string& name, sem_t* left, sem_t* right, int leftForkId, int rightForkId, sem_t* diningTable, pthread_barrier_t* startBarrier, Table* table, Mode mode) {
    this->id = id;
    this->name = name;
    this->leftFork = left;
    this->rightFork = right;
    this->leftForkId = leftForkId;
    this->rightForkId = rightForkId;
    this->diningTable = diningTable;
    this->startBarrier = startBarrier;
    this->table = table;
    this->mode = mode;
}


void Philosopher::start() {
    pthread_create(&thread, nullptr, threadEntry, this);
}


void Philosopher::think() {
    {
        std::lock_guard<std::mutex> lock(table->shared.mutex);
        table->shared.philosopherState[id] = 1;
    }
    sleep(rand() % 4 + 2);
}

void Philosopher::hungry() {
    printf("Filozof %s jest głodny.\n", name.c_str());
    {
        std::lock_guard<std::mutex> lock(table->shared.mutex);
        table->shared.philosopherState[id] = 2;
    }

}

void Philosopher::eat() {
    {
        std::lock_guard<std::mutex> lock(table->shared.mutex);
        table->shared.philosopherState[id] = 3;   // EATING
        table->shared.mealsEaten[id]++;           // licznik do UI
    }
    printf("Filozof %s je.\n", name.c_str());
    sleep(rand() % 4 + 1);
}


void *Philosopher::threadEntry(void *arg) {
    Philosopher* self = static_cast<Philosopher*>(arg);
    self->run();
    return nullptr;
}

void Philosopher::run() {


    // printf("Filozof %s doszedł do bariery.\n", name.c_str());
    pthread_barrier_wait(startBarrier);
    // printf("Filozof %s wystartował!\n", name.c_str());

    switch (mode) {
        case Mode::NAIVE:
            run_naive();
            break;
        case Mode::STARVATION:
            run_starvation();
            break;
        case Mode::SAFE:
            run_safe();
            break;
    }
}

void Philosopher::run_naive() {
    while (true) {
        hungry();

        sem_wait(leftFork);
        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[leftForkId] = id;   // mam lewą
        }
        printf("%s podniósł lewą pałeczkę.\n", name.c_str());



        sem_wait(rightFork);
        {sem_wait(leftFork);
        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[leftForkId] = id;   // mam lewą
        }
        printf("%s podniósł lewą pałeczkę.\n", name.c_str());
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[rightForkId] = id;  // mam prawą
        }
        printf("%s podniósł prawą pałeczkę.\n", name.c_str());

        eat();
        mealsEaten++;
        printf("-> %s zakończył posiłek (łącznie: %d).\n", name.c_str(), mealsEaten);

        // najpierw wizualnie, potem robię sem_post
        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[rightForkId] = -1;
        }
        sem_post(rightFork);

        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[leftForkId] = -1;
        }
        sem_post(leftFork);

        think();
    }
}


void Philosopher::run_starvation() {
    while (true) {
        hungry();

        sem_wait(diningTable);

        sem_wait(leftFork);
        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[leftForkId] = id;
        }

        sem_wait(rightFork);
        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[rightForkId] = id;
            table->shared.philosopherState[id]    = 3; // EATING (zielony)
        }

        eat();
        mealsEaten++;

        // Odkładanie: prawa -> lewa
        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[rightForkId] = -1;
        }
        sem_post(rightFork);

        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[leftForkId] = -1;
        }
        sem_post(leftFork);


        sem_post(diningTable);
        think();
    }
}







void Philosopher::run_safe() {
    while (true) {
        hungry();

        while (table->nextPhilosopher.load() != id) {
            usleep(1000);
        }

        sem_wait(&table->diningTable);

        sem_wait(leftFork);
        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[leftForkId] = id;
        }
        printf("%s podniósł lewą pałeczkę.\n", name.c_str());

        sem_wait(rightFork);
        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[rightForkId] = id;
        }
        printf("%s podniósł prawą pałeczkę.\n", name.c_str());

        eat();
        mealsEaten++;
        printf("-> %s zakończył posiłek (łącznie: %d).\n", name.c_str(), mealsEaten);

        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[rightForkId] = -1;
        }
        sem_post(rightFork);

        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            table->shared.forkOwners[leftForkId] = -1;
        }
        sem_post(leftFork);

        table->nextPhilosopher.store((id + 1) % 5);
        sem_post(&table->diningTable);

        think();
    }
}











