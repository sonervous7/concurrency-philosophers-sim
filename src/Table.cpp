//
// Created by sonervous7 on 30.10.2025.
//

#include "../include/Table.h"
#include <iostream>
#include <unistd.h>   // dla sleep
#include <ctime>      // dla srand(time(NULL))
#include <thread>

#include "Visualization.h"

using namespace std;

Table::Table(Mode mode) {
    srand(time(NULL));  // losowe czasy dla myślenia i jedzenia
    pthread_barrier_init(&startBarrier, nullptr, N);
    sem_init(&diningTable, 0, N-1);
    initPhilosophers(mode); // inicjalizacja pałeczek i filozofów
}

void Table::initPhilosophers(Mode mode) {
    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);
    }

    string names[N] = {"Yoda", "Gandalf", "Darth Vader", "Dumbledore", "Voldemort"};

    shared.forkOwners.assign(N, -1);
    shared.philosopherState.assign(N, 1);
    shared.mealsEaten.assign(N, 0);
    shared.names.assign(names, names + N);

    for (int i = 0; i < N; i++) {
        sem_t* left = &forks[i];
        sem_t* right = &forks[(i + 1) % N]; // modulo — żeby 4 miał pałeczkę 0

        philosophers.emplace_back(i, names[i], left, right, i, (i+1) % N, &diningTable, &startBarrier, this, mode);
    }

}

void Table::startDinner() {
    cout << "🍽️  Rozpoczynamy ucztę filozofów!\n";

    // Uruchamiamy wszystkich filozofów
    for (auto& p : philosophers) {
        p.start();
    }

    std::thread visThread(runVisualization, this);

    // Program pracuje dopóki użytkownik go nie przerwie (Ctrl+C)
    while (true) {
        sleep(10);
    }

    visThread.join();
}
