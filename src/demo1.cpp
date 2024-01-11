#include "General.hpp"
#include "Window.hpp"
#include "Mouse.hpp"

int main(int argc, char* args[]) {
    npp::init();

    npp::mwin.wstr(npp::mwin.wintp(npp::mwin.wstrp(0, npp::mwin.wstrx(0, 0, L"look: ⇳⇳⇳⇳⇳ "), L"№№№№№№№№№ - ∯", 7, "", {2, 4}), -129, 0), L" " + npp::mwin.gstr(1, 0, 20, 2, "boit"), 2, "st");
    npp::mwin.gchar();

    for (unsigned short i = 0; i < npp::mwin.gdimy(); i++) {
        for (unsigned short j = 0; j < npp::mwin.gdimx(); j++) {
            npp::mwin.wstr(i, j, L"#", rand() % 8);
        }
    }
    npp::mwin.rrad(1);
    npp::mwin.gchar();

    npp::mwin.reset();
    npp::mwin.wmstrp({30, 10}, "Hello There :) abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890`~!@#$%^&*()-_=+[{]}\\|;:'\",<.>/?", 2, true, 1, 1, {0, 1}, false);
    npp::mwin.gchar();

    return npp::end();
}
