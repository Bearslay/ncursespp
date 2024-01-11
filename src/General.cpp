#include "General.hpp"

bool npp::init(bool useMouse = false) {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(0);
    raw();
    srand(time(NULL));

    if (!has_colors() || !can_change_color()) {
        mvwprintw(stdscr, LINES / 2 - 1, COLS / 2 - 50, "Warning: Your terminal doesn't support full color capabilites...");
        mvwprintw(stdscr, LINES / 2, COLS / 2 - 50, "ncursespp requires a terminal that allows for the changing of colors, so go find one that will work");
        mvwprintw(stdscr, LINES / 2 + 1, COLS / 2 - 50, "Press any key to continue...");
        wgetch(stdscr);
        wclear(stdscr);

        return false;
    }

    start_color();

    init_color(0, 0, 0, 0);        // Black
    init_color(1, 999, 999, 999);  // White
    init_color(2, 999, 0, 0);      // Red
    init_color(3, 0, 999, 0);      // Green
    init_color(4, 999, 999, 0);    // Yellow
    init_color(5, 0, 0, 999);      // Blue
    init_color(6, 999, 0, 999);    // Magenta
    init_color(7, 0, 999, 999);    // Cyan

    init_pair(0, 1, 0);
    for (unsigned char i = 0; i < 8; i++) {
        init_pair(i + 1, i + 1, 0);
    }

    if (useMouse) {
        printf("\033[?1003h\n");
        mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    }

    mwin = Window();

    return true;
}

int npp::end(bool useMouse = false, int funcReturn = 0) {
    endwin();
    return funcReturn;
}
