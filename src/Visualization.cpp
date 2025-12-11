#include "../include/Visualization.h"
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <cmath>
#include <vector>
#include <string>
#include <array>
#include <algorithm>

static void drawLegend(int baseY) {
    mvprintw(baseY + 0, 0, "Legend:");
    attron(COLOR_PAIR(1)); mvprintw(baseY + 1, 2, "[ ] THINKING (white)");  attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2)); mvprintw(baseY + 2, 2, "[ ] HUNGRY   (red)");    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(3)); mvprintw(baseY + 3, 2, "[ ] EATING   (green)");  attroff(COLOR_PAIR(3));
    mvprintw(baseY + 5, 0, "Press 'q' to quit");
}

void runVisualization(Table* table) {
    // --- ncurses init ---
    initscr(); cbreak(); noecho(); curs_set(0);
    nodelay(stdscr, TRUE); keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // THINK
    init_pair(2, COLOR_RED,   COLOR_BLACK); // HUNGRY
    init_pair(3, COLOR_GREEN, COLOR_BLACK); // EAT

    // liczba filozofów
    int N = 5;
    {
        std::lock_guard<std::mutex> lock(table->shared.mutex);
        if (!table->shared.names.empty()) N = (int)table->shared.names.size();
    }

    // odczyt rozmiaru okna i dopasowanie elipsy jednorazowo (statyczny układ na tę sesję)
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    const int MARGIN_Y = 2, MARGIN_X = 2;
    const int BOX_W = 11;
    const char* TOP = "+---------+";
    const char* BOT = "+---------+";

    int CENTER_Y = rows / 2;
    int CENTER_X = cols / 2;
    int RADIUS_X = std::max(6, (cols - 2 * MARGIN_X - BOX_W) / 2);
    int RADIUS_Y = std::max(6, (rows - 2 * MARGIN_Y - 8) / 2);

    // precompute stałych pozycji
    std::vector<std::array<int,3>> deg(N); // [0]=angle, [1]=y, [2]=x
    for (int i = 0; i < N; ++i) {
        deg[i][0] = (360 / N) * i;
        double a  = deg[i][0] * M_PI / 180.0;
        deg[i][1] = CENTER_Y + (int)(RADIUS_Y * std::sin(a)); // Y
        deg[i][2] = CENTER_X + (int)(RADIUS_X * std::cos(a)); // X
    }

    auto clipLabel = [&](const std::string& s)->std::string {
        const int MAXW = BOX_W;                // imię nie szersze niż pudełko
        if ((int)s.size() <= MAXW) return s;
        if (MAXW <= 3) return s.substr(0, MAXW);
        return s.substr(0, MAXW - 3) + "...";
    };

    // pętla
    while (true) {
        int ch = getch();
        if (ch == 'q' || ch == 'Q') break;

        // snapshot współdzielonego stanu
        std::vector<int> forkOwners, state, meals;
        std::vector<std::string> names;
        {
            std::lock_guard<std::mutex> lock(table->shared.mutex);
            forkOwners = table->shared.forkOwners;       // -1 lub id posiadacza
            state      = table->shared.philosopherState; // 1/2/3
            meals      = table->shared.mealsEaten;
            names      = table->shared.names;
        }
        auto fixSize = [N](auto& v, auto fill){
            if ((int)v.size() < N) v.resize(N, fill);
            if ((int)v.size() > N) v.resize(N);
        };
        fixSize(forkOwners, -1);
        fixSize(state,       1);
        fixSize(meals,       0);
        if ((int)names.size() < N) names.resize(N, "Philosopher");

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        clear();

        // Forki
        for (int i = 0; i < N; ++i) {
            if (forkOwners[i] == -1) {
                attron(COLOR_PAIR(1));
                mvprintw(deg[i][1] + 2, deg[i][2] - 1, "%d", i);
                attroff(COLOR_PAIR(1));
            } else if (forkOwners[i] == i) {
                attron(COLOR_PAIR(3));
                mvprintw(deg[i][1] + 2, deg[i][2] - 1, "%d", i);
                attroff(COLOR_PAIR(3));
            } else {
                int owner = forkOwners[i];
                if (owner >= 0 && owner < N) {
                    attron(COLOR_PAIR(3));
                    mvprintw(deg[owner][1] + 2, deg[owner][2] + 3, "%d", i);
                    attroff(COLOR_PAIR(3));
                }
            }
        }

        // Filozofowie imię
        for (int i = 0; i < N; ++i) {
            int c = state[i];
            if (c < 1 || c > 3) c = 1;
            attron(COLOR_PAIR(c));

            std::string label = clipLabel(names[i]);
            mvprintw(deg[i][1] - 3, deg[i][2] - (int)label.size()/2, "%s", label.c_str());

            mvprintw(deg[i][1] - 1, deg[i][2] - (BOX_W/2), "%s", TOP);
            mvprintw(deg[i][1],     deg[i][2] - (BOX_W/2), "| %3d      |", std::min(meals[i], 999));
            mvprintw(deg[i][1] + 1, deg[i][2] - (BOX_W/2), "%s", BOT);

            attroff(COLOR_PAIR(c));
        }

        // legenda
        int rows2, cols2; getmaxyx(stdscr, rows2, cols2);
        int legendY = std::min(rows2 - 6, CENTER_Y + RADIUS_Y + 2);
        drawLegend(legendY);

        refresh();
    }

    endwin();
}
