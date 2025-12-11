#include "include/Table.h"
#include <iostream>
#include <ctime>

using namespace std;

int main() {
    setvbuf(stdout, NULL, _IONBF, 0); // wyłączenie buforowania stdout
    srand(time(NULL));

    cout << "=== Problem Uczty Filozofów ===" << endl;
    cout << "Wybierz wariant symulacji:" << endl;
    cout << "1. Klasyczny (możliwy deadlock)" << endl;
    cout << "2. Z zagłodzeniem (starvation)" << endl;
    cout << "3. Bez zakleszczeń i zagłodzenia (safe)" << endl;
    cout << "--------------------------------" << endl;
    cout << "Twój wybór: ";

    int wybor;
    cin >> wybor;

    Mode mode = Mode::NAIVE; // domyślnie klasyczny
    string opis;

    switch (wybor) {
        case 1:
            mode = Mode::NAIVE;
            opis = "Klasyczny (możliwy deadlock)";
            break;
        case 2:
            mode = Mode::STARVATION;
            opis = "Z zagłodzeniem (starvation)";
            break;
        case 3:
            mode = Mode::SAFE;
            opis = "Bez zakleszczeń i zagłodzenia (safe)";
            break;
        default:
            cout << "Nieprawidłowy wybór. Uruchamiam wariant klasyczny." << endl;
            mode = Mode::NAIVE;
            opis = "Klasyczny (możliwy deadlock)";
            break;
    }

    cout << endl << "🍽️  Uruchamianie wariantu: " << opis << "..." << endl;

    Table table(mode);
    table.startDinner();

    return 0;
}
