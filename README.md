# Concurrency Philosophers Sim

![Language](https://img.shields.io/badge/language-C++-00599C.svg)
![Platform](https://img.shields.io/badge/platform-Linux-FCC624.svg)
![License](https://img.shields.io/badge/license-MIT-green)

A multi-threaded simulation of the classic **Dining Philosophers Problem**, implemented in C++ on a Linux environment using CLion.

This project demonstrates synchronization techniques using **semaphores** and mutexes to manage shared resources (forks) among concurrent threads (philosophers). The core goal of this simulation is to benchmark and compare **three different resource access strategies** to prevent deadlocks and starvation.

## 🚀 Key Features

* **Multi-threading:** Implementation using POSIX threads (or C++ `std::thread`) to simulate concurrent behaviors.
* **Synchronization:** Usage of semaphores and mutexes for safe resource locking.
* **Comparative Analysis:** Side-by-side comparison of three distinct algorithms for resource acquisition.
* **Performance Metrics:** Tracks execution time, waiting times, and successfully eaten meals.

## 🧠 Implemented Strategies

The simulation benchmarks three approaches to solving the concurrency deadlock problem:

1.  **Strategy 1 (Naive Approach):** Philosophers attempt to grab the left fork, then the right fork concurrently. Used as a baseline to demonstrate deadlock scenarios.
2.  **Strategy 2 (Resource Hierarchy / Asymmetric):** Philosophers are assigned a strict order for picking up forks (e.g., odd-numbered philosophers pick left first, even-numbered pick right first) to prevent circular wait conditions.
3.  **Strategy 3 (Arbitrator / Access Limiting):** A semaphore restricts the number of philosophers eating simultaneously (e.g., allowing only N-1 philosophers at the table) to ensure at least one philosopher can always proceed.

## 🛠️ Technologies

* **Language:** C++ (Standard 17/20)
* **Build System:** CMake
* **IDE:** CLion
* **OS:** Ubuntu / Linux
* **Libraries:** `pthread` / `<semaphore>` / `<mutex>`

## 💻 Getting Started

### Prerequisites

Ensure you have the following installed on your Ubuntu system:

```bash
sudo apt update
sudo apt install build-essential cmake
```

## Building the Project
1. Clone the repository:
```bash
git clone [https://github.com/](https://github.com/)[sonervous7]/concurrency-philosophers-sim.git
cd concurrency-philosophers-sim
```
2. Create a build directory and compile:
```bash
mkdir build
cd build
cmake ..
make
```

## Running the Simulation
After compiling, you can run the executable.
```bash
./concurrency-philosophers-sim
```

## 📂 Project Structure
- `src/` - Source code (`.cpp` files).
- `include/` - Header files (`.h` files).
- `CMakeLists.txt` - CMake configuration file.
- `README.md` - Project Documentation
