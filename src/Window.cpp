#include "Window.hpp"

//
// INTERFACING WITH NCURSES
//

void npp::Window::write(unsigned short y, unsigned short x) {
    // No printing characters outside of the ncurses window
    if (y < 0 || y >= DimY || x < 0 || x >= DimX) {return;}

    wattron(Win, COLOR_PAIR(Grid[y][x].Color));
    toggleAttributes(Grid[y][x].Bold, Grid[y][x].Italic, Grid[y][x].Under, Grid[y][x].Rev, Grid[y][x].Blink, Grid[y][x].Dim, Grid[y][x].Invis, Grid[y][x].Stand, Grid[y][x].Prot, Grid[y][x].Alt);

    if (Grid[y][x].Char == '%') {mvwprintw(Win, y, x, "%%");}
    else {
        std::wstring str; str += Grid[y][x].Char;
        mvwaddwstr(Win, y, x, str.c_str());
    }

    wattroff(Win, COLOR_PAIR(Grid[y][x].Color));
    toggleAttributes(Grid[y][x].Bold, Grid[y][x].Italic, Grid[y][x].Under, Grid[y][x].Rev, Grid[y][x].Blink, Grid[y][x].Dim, Grid[y][x].Invis, Grid[y][x].Stand, Grid[y][x].Prot, Grid[y][x].Alt);
}

std::vector<bool> npp::Window::extractAttributes(std::string input) {
    for (unsigned int i = 0; i < input.length(); i++) {
        input[i] = tolower(input[i]);
    }

    std::vector<bool> output = {false, false, false, false, false, false, false, false, false, false};

    if (input.find("bo") != std::string::npos) {output[0] = true;}
    if (input.find("it") != std::string::npos) {output[1] = true;}
    if (input.find("un") != std::string::npos) {output[2] = true;}
    if (input.find("re") != std::string::npos) {output[3] = true;}
    if (input.find("bl") != std::string::npos) {output[4] = true;}
    if (input.find("di") != std::string::npos) {output[5] = true;}
    if (input.find("in") != std::string::npos) {output[6] = true;}
    if (input.find("st") != std::string::npos) {output[7] = true;}
    if (input.find("pr") != std::string::npos) {output[8] = true;}
    if (input.find("al") != std::string::npos) {output[9] = true;}

    return output;
}

void npp::Window::toggleAttributes(bool bold, bool italic, bool under, bool rev, bool blink, bool dim, bool invis, bool stand, bool prot, bool alt) {
    if (bold) {wattron(Win, A_BOLD);}
    else {wattroff(Win, A_BOLD);}
    if (italic) {wattron(Win, A_ITALIC);}
    else {wattroff(Win, A_ITALIC);}
    if (under) {wattron(Win, A_UNDERLINE);}
    else {wattroff(Win, A_UNDERLINE);}
    if (rev) {wattron(Win, A_REVERSE);}
    else {wattroff(Win, A_REVERSE);}
    if (blink) {wattron(Win, A_BLINK);}
    else {wattroff(Win, A_BLINK);}
    if (dim) {wattron(Win, A_DIM);}
    else {wattroff(Win, A_DIM);}
    if (invis) {wattron(Win, A_INVIS);}
    else {wattroff(Win, A_INVIS);}
    if (stand) {wattron(Win, A_STANDOUT);}
    else {wattroff(Win, A_STANDOUT);}
    if (prot) {wattron(Win, A_PROTECT);}
    else {wattroff(Win, A_PROTECT);}
    if (alt) {wattron(Win, A_ALTCHARSET);}
    else {wattroff(Win, A_ALTCHARSET);}
}
void npp::Window::toggleAttributes(std::string input) {
    std::vector<bool> att = extractAttributes(input);
    toggleAttributes(att[0], att[1], att[2], att[3], att[4], att[5], att[6], att[7], att[8], att[9]);
}

//
// TIMING
//

bool npp::Window::wait(unsigned long millis) {
    for (unsigned long i = 0; i < millis; i++) {
        if (CanSkip) {
            if (gchar(false, true, false) != ERR) {return true;}
        }
        napms(1);
    }

    return false;
}

//
// COMMON CHECKS & CODE SHORTCUTS
//

bool npp::Window::checkCoord(std::pair<unsigned short, unsigned short> pos) {return !(pos.first < 0 || pos.first >= DimY || pos.second < 0 || pos.second >= DimX);}
bool npp::Window::checkCoord(unsigned short y, unsigned short x) {return checkCoord({y, x});}

//
// LINE DRAWING HELPERS
//

const unsigned char npp::Window::getConnectStyle(unsigned short y, unsigned short x, unsigned char dir) {
    if (!checkCoord(y, x) || dir < 0 || dir > 3) {return 0;}

    // If the targeted cell can't be merged with (which is the case for most cells)
    // then there must not be something to connect with (even if the targeted cell is a box drawing element)
    if (!smerge(y, x)) {return 0;}
    
    wchar_t character = schar(y, x);
    for (unsigned char i = 0; i < Keys.LineDrawing.size(); i++) {
        if (character == Keys.LineDrawing[i].first) {return Keys.LineDrawing[i].second[dir];}
    }

    return 0;
}

const wchar_t npp::Window::getPiece(std::vector<unsigned char> dir, std::pair<unsigned char, unsigned char> style) {
    for (unsigned char i = 0; i < Keys.LineDrawing.size(); i++) {
        if (dir == Keys.LineDrawing[i].second) {
            // Vertical and horizontal lines (except for the doubled styles)
            if (i < 24) {
                if (style.first == HEAVY_HORIZONTAL) {return Keys.LineDrawing[i == 18 ? 12 + style.second : i + style.second].first;}
                else if (style.first == HEAVY_VERTICAL) {return Keys.LineDrawing[i == 6 ? style.second : i + style.second].first;}
                
                return Keys.LineDrawing[i + style.second].first;
            }
            // Vertical and horizontal lines for the doubled styles (they don't have dashed variants so are on their own)
            else if (i < 26) {
                if (style.first == DOUBLED_HORIZTONAL) {return Keys.LineDrawing[i == 24 ? i : style.second].first;}
                else if (style.first == DOUBLED_VERTICAL) {return Keys.LineDrawing[i == 24 ? 12 + style.second : i].first;}

                return Keys.LineDrawing[i].first;
            }
            // Corner pieces for the LIGHT_HARD and LIGHT_SOFT styles
            else if (i < 33) {
                return Keys.LineDrawing[i + (style.first == LIGHT_SOFT ? 1 : 0)].first;
            }

            return Keys.LineDrawing[i].first;
        }
    }

    return L' ';
}

npp::Window::Window(unsigned short y = 0, unsigned short x = 0, unsigned short dimy = LINES, unsigned short dimx = COLS) {
    // Prevent the window from being made out of bounds (and automatically resize ones that may)
    y = (y < 0 || y >= LINES) ? 0 : y;
    x = (x < 0 || x >= COLS) ? 0 : x;
    dimy = (dimy < 1 || LINES - dimy - y < 0) ? LINES - y : dimy;
    dimx = (dimx < 1 || COLS - dimx - x < 0) ? COLS - x : dimx;

    Win = newwin(dimy, dimx, y, x);
    PosY = y;
    PosX = x;
    DimY = dimy;
    DimX = dimx;

    for (unsigned short i = 0; i < DimY; i++) {
        Grid.emplace_back();
        for (unsigned short j = 0; j < DimX; j++) {
            Grid[i].emplace_back();
        }
    }
}
npp::Window::Window(unsigned short dimy, unsigned short dimx) {
    unsigned short y = LINES / 2 - dimy / 2;
    unsigned short x = COLS / 2 - dimx / 2;
    Window(y, x, dimy, dimx);
}
npp::Window::Window(Window &win, unsigned short dimy, unsigned short dimx) {
    dimy = (win.gdimy() < dimy) ? win.gdimy() : dimy;
    dimx = (win.gdimx() < dimx) ? win.gdimx() : dimx;
    unsigned short y = win.gposy() + win.gdimy() / 2 - dimy / 2;
    unsigned short x = win.gposx() + win.gdimx() / 2 - dimx / 2;
    Window(y, x, dimy, dimx);
}

//
// MISC
//

int npp::Window::rwait(unsigned long millis) {
    int input;

    for (unsigned long i = 0; i < millis; i++) {
        if (CanSkip) {
            input = gchar(false, true, false);

            if (input != ERR) {return input;}
        }
        napms(1);
    }

    return -1;
}

bool npp::Window::twait(unsigned long millis, std::vector<char> targets) {
    int input;

    for (unsigned long i = 0; i < millis; i++) {
        if (CanSkip) {
            input = gchar(false, true, false);

            if (input != ERR) {
                for (unsigned short j = 0; j < targets.size(); j++) {
                    if (input == targets[j]) {return true;}
                }
            }
        }
        napms(1);
    }

    return false;
}

void npp::Window::uskip(bool skippable) {CanSkip = skippable;}

void npp::Window::upreset() {PadUp = PadDown = PadLeft = PadRight = 0;}
void npp::Window::upup(unsigned short padding = 0) {PadUp = padding < 0 ? PadUp : padding;}
void npp::Window::updown(unsigned short padding = 0) {PadDown = padding < 0 ? PadDown : padding;}
void npp::Window::upleft(unsigned short padding = 0) {PadLeft = padding < 0 ? PadLeft : padding;}
void npp::Window::upright(unsigned short padding = 0) {PadRight = padding < 0 ? PadRight : padding;}
void npp::Window::upall(unsigned short padding = 0) {if (padding >= 0) {PadUp = PadDown = PadLeft = PadRight = padding;}}

//
// GETTING WINDOW/CELL ATTRIBUTES
//

const unsigned short npp::Window::gdimy() {return DimY;}
const unsigned short npp::Window::gdimx() {return DimX;}
const unsigned short npp::Window::gposy() {return PosY;}
const unsigned short npp::Window::gposx() {return PosX;}
const unsigned short npp::Window::gpadt() {return PadUp;}
const unsigned short npp::Window::gpadb() {return PadDown;}
const unsigned short npp::Window::gpadl() {return PadLeft;}
const unsigned short npp::Window::gpadr() {return PadRight;}

const wchar_t npp::Window::schar(unsigned short y, unsigned short x) {return !checkCoord(y, x) ? L' ' : Grid[y][x].Char;}
const unsigned char npp::Window::scolor(unsigned short y, unsigned short x) {return !checkCoord(y, x) ? 1 : Grid[y][x].Color;}
const bool npp::Window::sbold(unsigned short y, unsigned short x) {return !checkCoord(y, x) ? false : Grid[y][x].Bold;}
const bool npp::Window::sitalic(unsigned short y, unsigned short x) {return !checkCoord(y, x) ? false : Grid[y][x].Italic;}
const bool npp::Window::sunder(unsigned short y, unsigned short x) {return !checkCoord(y, x) ? false : Grid[y][x].Under;}
const bool npp::Window::srev(unsigned short y, unsigned short x) {return !checkCoord(y, x) ? false : Grid[y][x].Rev;}
const bool npp::Window::sblink(unsigned short y, unsigned short x) {return !checkCoord(y, x) ? false : Grid[y][x].Blink;}
const bool npp::Window::sdim(unsigned short y, unsigned short x) {return !checkCoord(y, x) ? false : Grid[y][x].Dim;}
const bool npp::Window::sinvis(unsigned short y, unsigned short x) {return !checkCoord(y, x) ? false : Grid[y][x].Invis;}
const bool npp::Window::sstand(unsigned short y, unsigned short x) {return !checkCoord(y, x) ? false : Grid[y][x].Stand;}
const bool npp::Window::sprot(unsigned short y, unsigned short x) {return !checkCoord(y, x) ? false : Grid[y][x].Prot;}
const bool npp::Window::salt(unsigned short y, unsigned short x) {return  !checkCoord(y, x) ? false : Grid[y][x].Alt;}
const bool npp::Window::smerge(unsigned short y, unsigned short x) {return !checkCoord(y, x) ? false : Grid[y][x].CanMerge;}

//
// WRITING TO WINDOW
//

void npp::Window::clear() {wclear(Win);}
void npp::Window::reset() {
    for (unsigned short i = 0; i < DimY; i++) {
        for (unsigned short j = 0; j < DimX; j++) {
            Grid[i][j] = Cell();
        }
    }
    wclear(Win);
}

std::pair<unsigned short, unsigned short> npp::Window::wcharp(std::pair<unsigned short, unsigned short> pos, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset) {
    if (!checkCoord(pos)) {return pos;}

    std::vector<bool> attributes = extractAttributes(att);

    Grid[pos.first][pos.second].Char = input;
    Grid[pos.first][pos.second].Color = color;
    
    Grid[pos.first][pos.second].Bold = attributes[0];
    Grid[pos.first][pos.second].Italic = attributes[1];
    Grid[pos.first][pos.second].Under = attributes[2];
    Grid[pos.first][pos.second].Rev = attributes[3];
    Grid[pos.first][pos.second].Blink = attributes[4];
    Grid[pos.first][pos.second].Dim = attributes[5];
    Grid[pos.first][pos.second].Invis = attributes[6];
    Grid[pos.first][pos.second].Stand = attributes[7];
    Grid[pos.first][pos.second].Prot = attributes[8];
    Grid[pos.first][pos.second].Alt = attributes[9];

    Grid[pos.first][pos.second].CanMerge = false;

    return {pos.first + offset.first, pos.second + offset.second};
}
std::pair<unsigned short, unsigned short> npp::Window::wcharp(unsigned short y, unsigned short x, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset) {return wcharp({y, x}, input, color, att, offset);}
unsigned short npp::Window::wchary(std::pair<unsigned short, unsigned short> pos, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset) {return wcharp(pos, input, color, att, offset).first;}
unsigned short npp::Window::wchary(unsigned short y, unsigned short x, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset) {return wchary({y, x}, input, color, att, offset);}
unsigned short npp::Window::wcharx(std::pair<unsigned short, unsigned short> pos, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset) {return wcharp(pos, input, color, att, offset).second;}
unsigned short npp::Window::wcharx(unsigned short y, unsigned short x, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset) {return wcharx({y, x}, input, color, att, offset);}
void npp::Window::wchar(std::pair<unsigned short, unsigned short> pos, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes) {wcharp(pos, input, color, att, {0, 0});}
void npp::Window::wchar(unsigned short y, unsigned short x, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes) {wchar({y, x}, input, color, att);}

std::pair<unsigned short, unsigned short> npp::Window::wstrp(std::pair<unsigned short, unsigned short> pos, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {
    if (!checkCoord(pos)) {return pos;}

    unsigned short yoffset = 0, xoffset = 0;
    for (unsigned short i = 0; i < input.length(); i++) {
        // Automatically wrap the string back to the starting x-pos
        if (pos.second + i - xoffset >= DimX) {
            yoffset++;
            xoffset += ((wrap) ? (DimX - pos.second) : DimX);
        }

        // Quit early if the string goes out of bounds
        if (!checkCoord(pos.first + yoffset, pos.second + i - xoffset)) {break;}

        wchar(pos.first + yoffset, pos.second + i - xoffset, input[i], color, att);
    }

    return {pos.first + yoffset + offset.first, pos.second + input.length() - 1 - xoffset + offset.second};
}
std::pair<unsigned short, unsigned short> npp::Window::wstrp(unsigned short y, unsigned short x, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wstrp({y, x}, input, color, att, offset, wrap);}
unsigned short npp::Window::wstry(std::pair<unsigned short, unsigned short> pos, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wstrp(pos, input, color, att, offset, wrap).first;}
unsigned short npp::Window::wstry(unsigned short y, unsigned short x, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wstry({y, x}, input, color, att, offset, wrap);}
unsigned short npp::Window::wstrx(std::pair<unsigned short, unsigned short> pos, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wstrp(pos, input, color, att, offset, wrap).second;}
unsigned short npp::Window::wstrx(unsigned short y, unsigned short x, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wstrx({y, x}, input, color, att, offset, wrap);}
void npp::Window::wstr(std::pair<unsigned short, unsigned short> pos, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, bool wrap = Defaults.Wrap) {wstrp(pos, input, color, att, {0, 0}, wrap);}
void npp::Window::wstr(unsigned short y, unsigned short x, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, bool wrap = Defaults.Wrap) {wstr({y, x}, input, color, att, wrap);}

std::pair<unsigned short, unsigned short> npp::Window::wintp(std::pair<unsigned short, unsigned short> pos, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {
    if (!checkCoord(pos)) {return pos;}

    // Convert number to a string that can be written to the window
    std::wstring num = std::to_wstring(input);
    unsigned short length = num.length();

    // Add leading zeros; negative numbers are accounted for by keeping the negative sign in front
    for (int i = 0; i < minWidth - length; i++) {
        num.insert(((input < 0) ? num.begin() + 1 : num.begin()), 1, '0');
    }

    return wstrp({pos.first, pos.second}, num, color, att, offset, wrap);
}
std::pair<unsigned short, unsigned short> npp::Window::wintp(unsigned short y, unsigned short x, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wintp({y, x}, input, minWidth, color, att, offset, wrap);}
unsigned short npp::Window::winty(std::pair<unsigned short, unsigned short> pos, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wintp(pos, input, minWidth, color, att, offset, wrap).first;}
unsigned short npp::Window::winty(unsigned short y, unsigned short x, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return winty({y, x}, input, minWidth, color, att, offset, wrap);}
unsigned short npp::Window::wintx(std::pair<unsigned short, unsigned short> pos, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wintp(pos, input, minWidth, color, att, offset, wrap).second;}
unsigned short npp::Window::wintx(unsigned short y, unsigned short x, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wintx({y, x}, input, minWidth, color, att, offset, wrap);}
void npp::Window::wint(std::pair<unsigned short, unsigned short> pos, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, bool wrap = Defaults.Wrap) {wintp(pos, input, minWidth, color, att, {0, 0}, wrap);}
void npp::Window::wint(unsigned short y, unsigned short x, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, bool wrap = Defaults.Wrap) {wint({y, x}, input, minWidth, color, att, wrap);}

std::pair<unsigned short, unsigned short> npp::Window::wmstrp(std::pair<unsigned short, unsigned short> pos, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {
    if (!checkCoord(pos)) {return pos;}
    kerning = kerning < 1 ? 1 : kerning;
    leading = leading < 1 ? 1 : leading;

    unsigned short posy = pos.first, posx = 0, xoffset = 0;
    bool dangling = false;

    for (unsigned short i = 0; i < input.length(); i++) {
        // Automatically wrap the string back to the starting x-pos
        if (posx + 3 > DimX - 4 - kerning) {
            posy += 3 + leading + (danglingLetters && leading < 2 ? 1 : 0);
            xoffset += wrap ? posx + 3 + kerning - pos.second : posx + 3 + kerning;
        }

        posx = pos.second + i * (3 + kerning) - xoffset;

        // Quit early if the string goes out of bounds
        if (!checkCoord(posy + 2, posx + 2)) {break;}

        for (unsigned char j = 0; j < Keys.MatrixText6x6.size(); j++) {
            if (input[i] == Keys.MatrixText6x6[j].first) {
                if (input[i] == 'g' || input[i] == 'j' || input[i] == 'p' || input[i] == 'q' || input[i] == 'y') {dangling = true;}

                for (unsigned char k = 0; k < 3; k++) {
                    for (unsigned char l = 0; l < 3; l++) {
                        wchar(posy + k + (dangling ? 1 : 0), posx + l, Keys.MatrixText6x6[j].second[k][l], color);
                    }
                }

                // Lowercase j needs a bit of extra work to fit in with the other lowercase letters since it has its dot
                if (input[i] == 'j') {wchar(wcharp(posy, posx + 1, L'▝', color, "", {1, 0}), L'▐', color);}

                dangling = false;
                break;
            }
        }
    }

    return {posy + 2 + offset.first, posx + 2 + offset.second};
}
std::pair<unsigned short, unsigned short> npp::Window::wmstrp(unsigned short y, unsigned short x, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wmstrp({y, x}, input, color, danglingLetters, kerning, leading, offset, wrap);}
unsigned short npp::Window::wmstry(std::pair<unsigned short, unsigned short> pos, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wmstrp(pos, input, color, danglingLetters, kerning, leading, offset, wrap).first;}
unsigned short npp::Window::wmstry(unsigned short y, unsigned short x, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wmstry({y, x}, input, color, danglingLetters, kerning, leading, offset, wrap);}
unsigned short npp::Window::wmstrx(std::pair<unsigned short, unsigned short> pos, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wmstrp(pos, input, color, danglingLetters, kerning, leading, offset, wrap).second;}
unsigned short npp::Window::wmstrx(unsigned short y, unsigned short x, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap) {return wmstry({y, x}, input, color, danglingLetters, kerning, leading, offset, wrap);}
void npp::Window::wmstr(std::pair<unsigned short, unsigned short> pos, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, bool wrap = Defaults.Wrap) {wmstrp(pos, input, color, danglingLetters, kerning, leading, {0, 0}, wrap);}
void npp::Window::wmstr(unsigned short y, unsigned short x, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, bool wrap = Defaults.Wrap) {wmstr({y, x}, input, color, danglingLetters, kerning, leading, wrap);}

//
// GET USER INPUT
//

int npp::Window::gchar(bool pause = true, bool enableKeypad = true, bool autoRender = true) {
    if (autoRender) {rinst();}

    if (enableKeypad) {keypad(Win, true);}
    else {keypad(Win, false);}
    if (pause) {nodelay(Win, false);}
    else {nodelay(Win, true);}

    return wgetch(Win);
}

std::wstring npp::Window::gstr(unsigned short y, unsigned short x, int maxChars = 255, unsigned char echoColor = Defaults.Color, std::string echoAtt = Defaults.Attributes, bool autoWrite = true, bool showStr = true, bool showCursor = true, bool enableKeypad = true) {
    wint_t input[maxChars];
    std::wstring output;

    if (enableKeypad) {keypad(Win, true);}
    if (showStr) {echo();}
    if (showCursor) {curs_set(1);}

    wattron(Win, COLOR_PAIR(echoColor));
    toggleAttributes(echoAtt);

    mvwgetn_wstr(Win, y, x, input, maxChars);

    wattroff(Win, COLOR_PAIR(echoColor));
    toggleAttributes(echoAtt);

    for (int i = 0; i < maxChars; i++) {
        if (input[i] == '\0') {break;}
        output += input[i];
    }

    if (autoWrite) {wstr(y, x, output, echoColor, echoAtt);}

    curs_set(0);
    noecho();
    keypad(Win, false);

    return output;
}

//
// RENDERING THE WINDOW
//

void npp::Window::rinst() {
    for (unsigned short i = 0; i < DimY; i++) {
        for (unsigned short j = 0; j < DimX; j++) {
            write(i, j);
        }
    }
    wrefresh(Win);
}

void npp::Window::rline(unsigned char dir = 0, bool full = true, bool rev = false, unsigned long millis = 20) {
    if (dir < 0 || dir > 3) {return rinst();}

    unsigned short l1 = dir < 2 ? DimY : DimX;
    unsigned short l2 = dir < 2 ? DimX : DimY;
    
    for (unsigned short i = 0; i < l1; i++) {
        for (unsigned short j = 0; j < l2; j++) {
            switch (dir) {
                case 0:
                    write(i, rev ? DimX - 1 - j : j);
                    break;
                case 1:
                    write(DimY - 1 - i, rev ? DimX - 1 - j : j);
                    break;
                case 2:
                    write(rev ? DimY - 1 - j : j, i);
                    break;
                case 3:
                    write(rev ? DimY - 1 - j : j, DimX - 1 -i);
                    break;
            }

            if (!full) {
                wrefresh(Win);
                if (wait(millis)) {return rinst();}
            }
        }
        
        wrefresh(Win);
        if (wait(millis)) {return rinst();}
    }
}
void npp::Window::rlinetop(bool full = true, bool rev = false, unsigned long millis = 20) {rline(0, full, rev, millis);}
void npp::Window::rlinebot(bool full = true, bool rev = false, unsigned long millis = 20) {rline(1, full, rev, millis);}
void npp::Window::rlineleft(bool full = true, bool rev = false, unsigned long millis = 20) {rline(2, full, rev, millis);}
void npp::Window::rlineright(bool full = true, bool rev = false, unsigned long millis = 20) {rline(3, full, rev, millis);}

void npp::Window::rrad(unsigned char divisions = 2, double angle = 90, bool ccw = true, unsigned long millis = 5, double resolution = 0.005) {
    // Fix the angle input and then convert it to radians
    angle = angle < 0 && !ccw ? angle + 90 : angle;
    while (angle < 0) {angle += 360;}
    angle = angle * (M_PI / 180);

    double divAngle = 2 * M_PI / divisions;
    double slope, cangle;
    unsigned char resMult;

    for (double i = angle; i < divAngle + angle; i += resolution) {
        // Each division will need to draw its own line and stuff
        for (unsigned char j = 0; j < divisions; j++) {
            // Find the angle that will be used to find the current slope
            cangle = i + divAngle * j;
            while (cangle >= M_PI * 2) {cangle -= M_PI * 2;}

            slope = (ccw ? -1 : 1) * ((DimX / 2) * sin(cangle)) / (DimX * cos(cangle));
            resMult = (cangle < M_PI / 2 + resolution * 16 && cangle > M_PI / 2 - resolution * 16) || (cangle < 3 * M_PI / 2 + resolution * 16 && cangle > 3 * M_PI / 2 - resolution * 16) ? 10 : 25;

            // Pretty much just draw a line from the middle of the window to the edge of the window along the calculated slope
            for (double k = 0; k <= DimX / 2; k+= resolution * resMult) {
                if (cangle < M_PI / 2 || cangle > 3 * M_PI / 2) {
                    write(DimY / 2 + slope * k, DimX / 2 + k * 2);
                    write(DimY / 2 + slope * k - 1, DimX / 2 + k * 2);
                    write(DimY / 2 + slope * k + 1, DimX / 2 + k * 2);
                    write(DimY / 2 + slope * k, DimX / 2 + k * 2 - 1);
                    write(DimY / 2 + slope * k, DimX / 2 + k * 2 + 2);
                } else {
                    write(DimY / 2 - slope * k, DimX / 2 - k * 2);
                    write(DimY / 2 - slope * k - 1, DimX / 2 - k * 2);
                    write(DimY / 2 - slope * k + 1, DimX / 2 - k * 2);
                    write(DimY / 2 - slope * k, DimX / 2 - k * 2 - 1);
                    write(DimY / 2 - slope * k, DimX / 2 - k * 2 + 2);
                }
            }
        }
    
        wrefresh(Win);
        if (wait(millis)) {return rinst();}
    }

    rinst();
}

//
// LINE DRAWING
//

void npp::Window::dline(unsigned short y, unsigned short x, unsigned short length, bool vertical = false, bool rev = false, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge) {
    if (!checkCoord(y, x) || length <= 0 || (y < 0 && rev) || (y >= DimY && !rev) || (x < 0 && rev) || (x >= DimX && !rev)) {return;}
    style.first = (style.first < 0 || style.first >= 8) ? LIGHT_HARD : style.first;
    style.second = (style.second < 0 || style.second >= 6) ? DASHED_NONE : style.second;

    unsigned short posy = y, posx = x;
    std::vector<unsigned char> dirs;
    wchar_t piece;
    unsigned char tdir1 = vertical ? (DIR_DOWN - (rev ? 1 : 0)) : (DIR_RIGHT - (rev ? 1 : 0));
    unsigned char tdir2 = tdir1 + (rev ? 1 : -1);

    /// @brief Used to fabricate a direction (connection style) based on the inputted style
    unsigned char cstyle;
    switch (style.first) {
        case LIGHT_HARD:
        case LIGHT_SOFT:
            cstyle = STYLE_LIGHT;
            break;
        case HEAVY_BOTH:
            cstyle = STYLE_HEAVY;
            break;
        case HEAVY_HORIZONTAL:
            cstyle = vertical ? STYLE_LIGHT : STYLE_HEAVY;
            break;
        case HEAVY_VERTICAL:
            cstyle = vertical ? STYLE_HEAVY : STYLE_LIGHT;
            break;
        case DOUBLED_BOTH:
            cstyle = STYLE_DOUBLED;
            break;
        case DOUBLED_HORIZTONAL:
            cstyle = vertical ? STYLE_LIGHT : STYLE_DOUBLED;
            break;
        case DOUBLED_VERTICAL:
            cstyle = vertical ? STYLE_DOUBLED : STYLE_LIGHT;
            break;
    }

    for (unsigned short i = 0; i < length; i++) {
        // Quit early if the line starts to venture out of the window
        if (!checkCoord(posy, posx)) {break;}

        // Get the directions (connection styles) of the surrounding characters (for connecting to existing lines)
        // This step is skipped if the line isn't suppsoed to merge with others
        if (canMerge) {dirs = {getConnectStyle(posy - 1, posx, DIR_DOWN), getConnectStyle(posy + 1, posx, DIR_UP), getConnectStyle(posy, posx - 1, DIR_RIGHT), getConnectStyle(posy, posx + 1, DIR_LEFT)};}
        else {dirs = {STYLE_NONE, STYLE_NONE, STYLE_NONE, STYLE_NONE};}

        // With the exception of the end pieces, the line has to have directions fabricated in order to actually exist
        if (i == 0) {dirs[tdir2] = (dirs[tdir2] == STYLE_NONE && dirs[vertical ? DIR_LEFT : DIR_UP] == STYLE_NONE && dirs[vertical ? DIR_RIGHT : DIR_DOWN] == STYLE_NONE) ? cstyle : dirs[tdir2];}
        else {dirs[tdir2] = cstyle;}
        if (i == length - 1) {dirs[tdir1] = (dirs[tdir1] == STYLE_NONE && dirs[vertical ? DIR_LEFT : DIR_UP] == STYLE_NONE && dirs[vertical ? DIR_RIGHT : DIR_DOWN] == STYLE_NONE) ? cstyle : dirs[tdir1];}
        else {dirs[tdir1] = cstyle;}

        piece = getPiece(dirs, style);

        // In the scenario that a doubled line intersects with a heavy one, the directions need to be altered because there are no
        // unicode characters that properly mesh the two styles; the current style is kept because it is the most recent one being used
        if (piece == L' ') {
            // If the current style is heavy, then all doubled directions are replaced with heavy directions
            // The opposite is done if the current style is doubled
            for (unsigned char i = 0; i < 4; i++) {
                if (style.first == HEAVY_BOTH || style.first == HEAVY_HORIZONTAL || style.first == HEAVY_VERTICAL) {dirs[i] = (dirs[i] == STYLE_DOUBLED) ? STYLE_HEAVY : dirs[i];}
                else {dirs[i] = (dirs[i] == STYLE_HEAVY) ? STYLE_DOUBLED : dirs[i];}
            }

            piece = getPiece(dirs, style);
        }

        wchar(posy, posx, piece, color);
        Grid[posy][posx].CanMerge = mergeable;

        posy = y + (rev ? -1 : 1) * (i + 1) * (vertical ? 1 : 0);
        posx = x + (rev ? -1 : 1) * (i + 1) * (vertical ? 0 : 1);
    }
}
void npp::Window::dvline(unsigned short y, unsigned short x, unsigned short length, bool rev = false, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge) {dline(y, x, length, true, rev, style, color, mergeable, canMerge);}
void npp::Window::dhline(unsigned short y, unsigned short x, unsigned short length, bool rev = false, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge) {dline(y, x, length, false, rev, style, color, mergeable, canMerge);}

void npp::Window::dbox(unsigned short y, unsigned short x, unsigned short dimy, unsigned short dimx, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge) {                
    if (!checkCoord(y, x) || !checkCoord(y + dimy, x + dimx) || dimy < 1 || dimx < 1) {return;}

    // Special cases for when the dimensions of the box are 1
    if (dimy == 1 && dimx == 1) {return wstr(y, x, L"□", color);}
    else if (dimy == 1) {return dhline(y, x, dimx, false, style, color, mergeable, canMerge);}
    else if (dimx == 1) {return dvline(y, x, dimy, false, style, color, mergeable, canMerge);}

    dvline(y, x, dimy, false, style, color, mergeable, canMerge);
    dvline(y, x + dimx - 1, dimy, false, style, color, mergeable, canMerge);
    dhline(y, x, dimx, false, style, color, mergeable, canMerge);
    dhline(y + dimy - 1, x, dimx, false, style, color, mergeable, canMerge);
}
void npp::Window::dbox(std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge) {dbox(0, 0, DimY, DimX, style, color, false, false);}
void npp::Window::dcbox(unsigned short y, unsigned short x, unsigned short dimy, unsigned short dimx, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge) {dbox(y - dimy / 2, x - dimx / 2, dimy, dimx, style, color, mergeable, canMerge);}

void npp::Window::dgrid(unsigned short y, unsigned short x, unsigned short rows, unsigned short cols, unsigned short celly, unsigned short cellx, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = true) {
    std::pair<unsigned short, unsigned short> dims = gGridDims(rows, cols, celly, cellx);

    if (!checkCoord(y, x) || !checkCoord(dims.first, dims.second) || rows < 1 || cols < 1 || celly < 0 || cellx < 0) {return;}

    // Vertical lines
    for (unsigned short i = 0; i <= cols; i++) {
        dvline(y, x + cellx * i + i, dims.first, false, style, color, mergeable, canMerge);
    }
    // Horizontal lines
    for (unsigned short i = 0; i <= rows; i++) {
        dhline(y + celly * i + i, x, dims.second, false, style, color, mergeable, canMerge);
    }
}
void npp::Window::dgrid(unsigned short y, unsigned short x, unsigned short cells, unsigned short cellDim, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge) {dgrid(y, x, cells, cells, cellDim, cellDim * 2, style, color, mergeable, canMerge);}
void npp::Window::dcgrid(unsigned short y, unsigned short x, unsigned short rows, unsigned short cols, unsigned short celly, unsigned short cellx, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge) {dgrid(y - gGridDims(rows, cols, celly, cellx).first, x - gGridDims(rows, cols, celly, cellx).second, rows, cols, celly, cellx, style, color, mergeable, canMerge);}
void npp::Window::dcgrid(unsigned short y, unsigned short x, unsigned short cells, unsigned short cellDim, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge) {dcgrid(y, x, cells, cells, cellDim, cellDim * 2, style, color, mergeable, canMerge);}

std::pair<unsigned short, unsigned short> npp::Window::gGridDims(unsigned short rows, unsigned short cols, unsigned short celly, unsigned short cellx) {return {rows * celly + (rows + 1), cols * cellx + (cols + 1)};}
