#include "Mouse.hpp"

const char npp::Mouse::ginput() {
    return Button;
}

const unsigned short npp::Mouse::gx() {
    return X;
}

const unsigned short npp::Mouse::gy() {
    return Y;
}

const unsigned short npp::Mouse::gz() {
    return Z;
}

bool npp::Mouse::gmouse(int input) {
    MEVENT event;
    if (getmouse(&event) != OK) {return false;}

    // Change the janky ncurses values for the mouse into easier ones
    // Mouse 2 is actually bound to right click as well, not middle click
    switch (event.bstate) {
        case BUTTON1_PRESSED:
            Button = M1_PRESS;
            break;
        case BUTTON1_RELEASED:
            Button = M1_RELEASE;
            break;
        case BUTTON1_CLICKED:
            Button = M1_CLICK;
            break;
        case BUTTON1_DOUBLE_CLICKED:
            Button = M1_DCLICK;
            break;
        case BUTTON1_TRIPLE_CLICKED:
            Button = M1_TCLICK;
            break;
        case BUTTON2_PRESSED:
            Button = M3_PRESS;
            break;
        case BUTTON2_RELEASED:
            Button = M3_RELEASE;
            break;
        case BUTTON2_CLICKED:
            Button = M3_CLICK;
            break;
        case BUTTON2_DOUBLE_CLICKED:
            Button = M3_DCLICK;
            break;
        case BUTTON2_TRIPLE_CLICKED:
            Button = M3_TCLICK;
            break;
        case BUTTON3_PRESSED:
            Button = M2_PRESS;
            break;
        case BUTTON3_RELEASED:
            Button = M2_RELEASE;
            break;
        case BUTTON3_CLICKED:
            Button = M2_CLICK;
            break;
        case BUTTON3_DOUBLE_CLICKED:
            Button = M2_DCLICK;
            break;
        case BUTTON3_TRIPLE_CLICKED:
            Button = M2_TCLICK;
            break;
        case BUTTON4_PRESSED:
            Button = M4_PRESS;
            break;
        case BUTTON4_RELEASED:
            Button = M4_RELEASE;
            break;
        case BUTTON4_CLICKED:
            Button = M4_CLICK;
            break;
        case BUTTON4_DOUBLE_CLICKED:
            Button = M4_DCLICK;
            break;
        case BUTTON4_TRIPLE_CLICKED:
            Button = M4_TCLICK;
            break;
        case BUTTON5_PRESSED:
            Button = M5_PRESS;
            break;
        case BUTTON5_RELEASED:
            Button = M5_RELEASE;
            break;
        case BUTTON5_CLICKED:
            Button = M5_CLICK;
            break;
        case BUTTON5_DOUBLE_CLICKED:
            Button = M5_DCLICK;
            break;
        case BUTTON5_TRIPLE_CLICKED:
            Button = M5_TCLICK;
            break;
        default:
            Button = M_UNKNOWN;
            break;
    }

    X = event.x;
    Y = event.y;
    Z = event.z;

    return true;
}
