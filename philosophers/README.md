# Dining Philosophers – C++20, CMake, ncurses

Wizualna symulacja klasycznego problemu **Filozofów u stołu** (Dining Philosophers) w C++ z użyciem wątków POSIX, semaforów i biblioteki **ncurses**.  
Projekt zawiera trzy warianty algorytmów: **NAIVE** (możliwy deadlock), **STARVATION** (możliwe zagłodzenie), **SAFE** (bez deadlocku i bez zagłodzenia).

> **Uwaga:** Uruchamiaj w **zewnętrznym terminalu**, nie wbudowanej konsoli IDE – `ncurses` potrafi „migać” lub nie działać poprawnie w konsoli CLion.

---

## Wymagania

### Linux / WSL (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install -y clang cmake make \
    libncurses5-dev libncursesw5-dev
```

## Budowanie (clang + CMake)

cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++

cmake --build build -j


## Uruchomienie
./build/philosophers

## Tryby działania

W projekcie są trzy tryby; wybór zwykle odbywa się w konstruktorze `Table` lub przy starcie w `main.cpp` (np. `Table table(Mode::STARVATION);`):

- **NAIVE** – *możliwy deadlock*  
  Każdy filozof **blokująco** pobiera lewą pałeczkę, potem **blokująco** prawą. Wspólny start i równa kolejność prowadzi często do cyklu „trzymam jedną i czekam na drugą”, czyli **zakleszczenia**.

- **STARVATION** – *możliwe zagłodzenie*  
  Wersja z tzw. **kelnerem N−1** (przy stole max `N-1`), **blokujące** pobieranie pałeczek. Deadlocka nie ma, ale z racji braku gwarancji FIFO w semaforach kelnera może wystąpić **niesprawiedliwość** – po dłuższym czasie jeden filozof bywa wyraźnie „chudszy”.

- **SAFE** – *bez deadlocku i bez zagłodzenia*  
  Wariant zapewniający **sprawiedliwość** (np. kelner z kolejką FIFO / round-robin / porządek zasobów). Każdy w skończonym czasie dostaje możliwość jedzenia.

## Legenda i odczyt wizualizacji

- **Kolory stanów:**
    - **biały** – THINKING (myśli),
    - **czerwony/żółty** – HUNGRY (głodny, czeka na komplet pałeczek),
    - **zielony** – EATING (je).

- **Licznik posiłków:** mały kwadrat/ramka przy każdym filozofie pokazuje **liczbę zjedzonych porcji**.

- **Pałeczki (forki):** numery przy pozycjach pałeczek wskazują **aktualnego właściciela** danej pałeczki; brak numeru = pałeczka leży na stole (wolna).
