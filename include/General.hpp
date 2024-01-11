#pragma once

#include <ncurses.h>
#include <vector>
#include <utility>
#include <locale.h>
#include <string>
#include <math.h>
#include <cmath>
#include <ctime>

#include "Window.hpp"
#include "Mouse.hpp"

/// @brief Unknown mouse input
#define M_UNKNOWN -1
/// @brief Mouse 1 (Left Click) being pressed
#define M1_PRESS 0
/// @brief Mouse 1 (Left Click) being released
#define M1_RELEASE 1
/// @brief Mouse 1 (Left Click) being clicked once quickly
#define M1_CLICK 2
/// @brief Mouse 1 (Left Click) being clicked twice quickly
#define M1_DCLICK 3
/// @brief Mouse 1 (Left Click) being clicked thrice quickly
#define M1_TCLICK 4
/// @brief Mouse 2 (Right Click) being pressed
#define M2_PRESS 5
/// @brief Mouse 2 (Right Click) being released
#define M2_RELEASE 6
/// @brief Mouse 2 (Right Click) being clicked once quickly
#define M2_CLICK 7
/// @brief Mouse 2 (Right Click) being clicked twice quickly
#define M2_DCLICK 8
/// @brief Mouse 2 (Right Click) being clicked thrice quickly
#define M2_TCLICK 9
/// @brief Mouse 3 (Middle Click) being pressed
#define M3_PRESS 10
/// @brief Mouse 3 (Middle Click) being released
#define M3_RELEASE 11
/// @brief Mouse 3 (Middle Click) being clicked once quickly
#define M3_CLICK 12
/// @brief Mouse 3 (Middle Click) being clicked twice quickly
#define M3_DCLICK 13
/// @brief Mouse 3 (Middle Click) being clicked thrice quickly
#define M3_TCLICK 14
/// @brief Mouse 4 (Scroll Up) being pressed (ACTUAL INPUT - USE THIS FOR SCROLLING)
#define M4_PRESS 15
/// @brief Mouse 4 (Scroll Up) being released (DOESN'T DO ANYTHING)
#define M4_RELEASE 16
/// @brief Mouse 4 (Scroll Up) being clicked once quickly (DOESN'T DO ANYTHING)
#define M4_CLICK 17
/// @brief Mouse 4 (Scroll Up) being clicked twice quickly (DOESN'T DO ANYTHING)
#define M4_DCLICK 18
/// @brief Mouse 4 (Scroll Up) being clicked thrice quickly (DOESN'T DO ANYTHING)
#define M4_TCLICK 19
/// @brief Mouse 5 (Scroll Down) being pressed (ACTUAL INPUT - USE THIS FOR SCROLLING)
#define M5_PRESS 20
/// @brief Mouse 5 (Scroll Down) being released (DOESN'T DO ANYTHING)
#define M5_RELEASE 21
/// @brief Mouse 5 (Scroll Down) being clicked once quickly (DOESN'T DO ANYTHING)
#define M5_CLICK 22
/// @brief Mouse 5 (Scroll Down) being clicked twice quickly (DOESN'T DO ANYTHING)
#define M5_DCLICK 23
/// @brief Mouse 5 (Scroll Down being clicked thrice quickly (DOESN'T DO ANYTHING)
#define M5_TCLICK 24

/// @brief Light box drawing characters with square corners
#define LIGHT_HARD 0
/// @brief Light box drawing characters with rounded corners
#define LIGHT_SOFT 1
/// @brief Heavy box drawing characters in both the horiztonal and vertical directions
#define HEAVY_BOTH 2
/// @brief Heavy box drawing characters in the horiztonal direction, light in the vertical directions
#define HEAVY_HORIZONTAL 3
/// @brief Heavy box drawing characters in the vertical direction, light in the horizontal directions
#define HEAVY_VERTICAL 4
/// @brief Doubled box drawing characters in both the horizontal and vertical directions
#define DOUBLED_BOTH 5
/// @brief Doubled box drawing characters in the horiztonal direction, light in the vertical directions
#define DOUBLED_HORIZTONAL 6
/// @brief Doubled box drawing characters in the vertical direction, light in the horizontal directions
#define DOUBLED_VERTICAL 7

/// @brief Solid Line
#define DASHED_NONE 0
/// @brief Dashed line with one gap per character
#define DASHED_DOUBLE 1
/// @brief Dashed line with two gaps per character
#define DASHED_TRIPLE 2
/// @brief Dashed line with three gap per character
#define DASHED_QUADRUPLE 3
/// @brief Half of a solid line used as a dashed line (left/up)
#define DASHED_SINGLE_1 4
/// @brief Half of a solid line used as a dashed line (right/down)
#define DASHED_SINGLE_2 5

/// @brief No connection style
#define STYLE_NONE 0
/// @brief A light connection style
#define STYLE_LIGHT 1
/// @brief A heavy connection style
#define STYLE_HEAVY 2
/// @brief A doubled connection style
#define STYLE_DOUBLED 3

/// @brief Up (used for extracting directions and connection styles from box drawing characters)
#define DIR_UP 0
/// @brief Down (used for extracting directions and connection styles from box drawing characters)
#define DIR_DOWN 1
/// @brief Left (used for extracting directions and connection styles from box drawing characters)
#define DIR_LEFT 2
/// @brief Right (used for extracting directions and connection styles from box drawing characters)
#define DIR_RIGHT 3

namespace npp {
    /// @brief Default values that can be changed in place of providing parameter arguments for many functions
    static struct {
        /// @brief Color pair
        unsigned char Color = 1;
        /// @brief The set of attributes to use by default when writing to the window
        std::string Attributes = "";
        /// @brief The pair of y (row) and x (col) offsets to use when chaining writing functions
        std::pair<unsigned short, unsigned short> Offset = {0, 1};
        /// @brief Whether to wrap to the starting x or to the window's x
        bool Wrap = true;
        /// @brief The minimum width of various numbers to have (in characters) when written to the window
        unsigned short MinWidth = 0;

        bool DanglingLetters = true;
        unsigned short Kerning = 1;
        unsigned short Leading = 1;

        std::pair<unsigned char, unsigned char> Style = {LIGHT_HARD, DASHED_NONE};
        bool Mergeable = true;
        bool CanMerge = true;
    } Defaults;

    /// @brief Various keys used to store and request information relevant to each key
    static const struct {
        /// @brief The key that matches unicode box drawing characters to the directions/styles that make them up
        std::vector<std::pair<wchar_t, std::vector<unsigned char>>> LineDrawing = {
            // Horizontal and vertical lines off all 6 dash styles (none, single 1, single 2, double, triple, quadruple)
            {L'─', {0, 0, 1, 1}}, {L'╌', {0, 0, 1, 1}}, {L'┄', {0, 0, 1, 1}}, {L'┈', {0, 0, 1, 1}}, {L'╴', {0, 0, 1, 1}}, {L'╶', {0, 0, 1, 1}},
            {L'━', {0, 0, 2, 2}}, {L'╍', {0, 0, 2, 2}}, {L'┅', {0, 0, 2, 2}}, {L'┉', {0, 0, 2, 2}}, {L'╸', {0, 0, 2, 2}}, {L'╺', {0, 0, 2, 2}},
            {L'│', {1, 1, 0, 0}}, {L'╎', {1, 1, 0, 0}}, {L'┆', {1, 1, 0, 0}}, {L'┊', {1, 1, 0, 0}}, {L'╵', {1, 1, 0, 0}}, {L'╷', {1, 1, 0, 0}},
            {L'┃', {2, 2, 0, 0}}, {L'╏', {2, 2, 0, 0}}, {L'┇', {2, 2, 0, 0}}, {L'┋', {2, 2, 0, 0}}, {L'╹', {2, 2, 0, 0}}, {L'╻', {2, 2, 0, 0}},
            {L'═', {0, 0, 3, 3}}, {L'║', {3, 3, 0, 0}},
            // Corners
            {L'┌', {0, 1, 0, 1}}, {L'╭', {0, 1, 0, 1}}, {L'┐', {0, 1, 1, 0}}, {L'╮', {0, 1, 1, 0}}, {L'└', {1, 0, 0, 1}}, {L'╰', {1, 0, 0, 1}}, {L'┘', {1, 0, 1, 0}}, {L'╯', {1, 0, 1, 0}}, 
            {L'┍', {0, 1, 0, 2}}, {L'┎', {0, 2, 0, 1}}, {L'┏', {0, 2, 0, 2}}, {L'╒', {0, 1, 0, 3}}, {L'╓', {0, 3, 0, 1}}, {L'╔', {0, 3, 0, 3}},
            {L'┑', {0, 1, 2, 0}}, {L'┒', {0, 2, 1, 0}}, {L'┓', {0, 2, 2, 0}}, {L'╕', {0, 1, 3, 0}}, {L'╖', {0, 3, 1, 0}}, {L'╗', {0, 3, 3, 0}},
            {L'┕', {1, 0, 0, 2}}, {L'┖', {2, 0, 0, 1}}, {L'┗', {2, 0, 0, 2}}, {L'╘', {1, 0, 0, 3}}, {L'╙', {3, 0, 0, 1}}, {L'╚', {3, 0, 0, 3}},
            {L'┙', {1, 0, 2, 0}}, {L'┚', {2, 0, 1, 0}}, {L'┛', {2, 0, 2, 0}}, {L'╛', {1, 0, 3, 0}}, {L'╜', {3, 0, 1, 0}}, {L'╝', {3, 0, 3, 0}},
            // Tees
            {L'├', {1, 1, 0, 1}}, {L'┝', {1, 1, 0, 2}}, {L'┞', {2, 1, 0, 1}}, {L'┟', {1, 2, 0, 1}}, {L'┠', {2, 2, 0, 1}}, {L'┡', {2, 1, 0, 2}}, {L'┢', {1, 2, 0, 2}}, {L'┣', {2, 2, 0, 2}}, {L'╞', {1, 1, 0, 3}}, {L'╟', {3, 3, 0, 1}}, {L'╠', {3, 3, 0, 3}},
            {L'┤', {1, 1, 1, 0}}, {L'┥', {1, 1, 2, 0}}, {L'┦', {2, 1, 1, 0}}, {L'┧', {1, 2, 1, 0}}, {L'┨', {2, 2, 1, 0}}, {L'┩', {2, 1, 2, 0}}, {L'┪', {1, 2, 2, 0}}, {L'┫', {2, 2, 2, 0}}, {L'╡', {1, 1, 3, 0}}, {L'╢', {3, 3, 1, 0}}, {L'╣', {3, 3, 3, 0}},
            {L'┬', {0, 1, 1, 1}}, {L'┭', {0, 1, 2, 1}}, {L'┮', {0, 1, 1, 2}}, {L'┯', {0, 1, 2, 2}}, {L'┰', {0, 2, 1, 1}}, {L'┱', {0, 2, 2, 1}}, {L'┲', {0, 2, 1, 2}}, {L'┳', {0, 2, 2, 2}}, {L'╤', {0, 1, 3, 3}}, {L'╥', {0, 3, 1, 1}}, {L'╦', {0, 3, 3, 3}},
            {L'┴', {1, 0, 1, 1}}, {L'┵', {1, 0, 2, 1}}, {L'┶', {1, 0, 1, 2}}, {L'┷', {1, 0, 2, 2}}, {L'┸', {2, 0, 1, 1}}, {L'┹', {2, 0, 2, 1}}, {L'┺', {2, 0, 1, 2}}, {L'┻', {2, 0, 2, 2}}, {L'╧', {1, 0, 3, 3}}, {L'╨', {3, 0, 1, 1}}, {L'╩', {3, 0, 3, 3}},
            // Crosses
            {L'┼', {1, 1, 1, 1}}, {L'┽', {1, 1, 2, 1}}, {L'┾', {1, 1, 1, 2}}, {L'┿', {1, 1, 2, 2}}, {L'╀', {2, 1, 1, 1}}, {L'╁', {1, 2, 1, 1}}, {L'╂', {2, 2, 1, 1}}, {L'╃', {2, 1, 2, 1}},
            {L'╄', {2, 1, 1, 2}}, {L'╅', {1, 2, 2, 1}}, {L'╆', {1, 2, 1, 2}}, {L'╇', {2, 2, 1, 2}}, {L'╈', {1, 2, 2, 2}}, {L'╉', {2, 2, 2, 1}}, {L'╊', {2, 2, 1, 2}}, {L'╋', {2, 2, 2, 2}},
            {L'╪', {1, 1, 3, 3}}, {L'╫', {3, 3, 1, 1}}, {L'╬', {3, 3, 3, 3}},
            // Thickness transitions
            {L'╼', {0, 0, 1, 2}}, {L'╽', {1, 2, 0, 0}}, {L'╾', {0, 0, 2, 1}}, {L'╿', {2, 1, 0, 0}}
        };

        /// @brief The key that matches a 3x3 set of unicode box element characters to the regular character they represent
        std::vector<std::pair<char, std::vector<std::vector<wchar_t>>>> MatrixText6x6 = {
            {' ', {{L' ', L' ', L' '}, {L' ', L' ', L' '}, {L' ', L' ', L' '}}},
            {'a', {{L' ', L' ', L' '}, {L'▞', L'▀', L'▟'}, {L'▚', L'▄', L'▜'}}},
            {'b', {{L'▌', L' ', L' '}, {L'▙', L'▀', L'▚'}, {L'▛', L'▄', L'▞'}}},
            {'c', {{L' ', L' ', L' '}, {L'▞', L'▀', L'▀'}, {L'▚', L'▄', L'▄'}}},
            {'d', {{L' ', L' ', L'▐'}, {L'▞', L'▀', L'▟'}, {L'▚', L'▄', L'▜'}}},
            {'e', {{L' ', L' ', L' '}, {L'▟', L'█', L'▙'}, {L'▚', L'▄', L'▄'}}},
            {'f', {{L' ', L'▞', L'▖'}, {L'▗', L'▙', L'▖'}, {L' ', L'▌', L' '}}},
            {'g', {{L'▞', L'▀', L'▟'}, {L'▚', L'▄', L'▜'}, {L'▗', L'▄', L'▞'}}},
            {'h', {{L'▌', L' ', L' '}, {L'▙', L'▀', L'▚'}, {L'▌', L' ', L'▐'}}},
            {'i', {{L' ', L'▘', L' '}, {L' ', L'▌', L' '}, {L' ', L'▚', L' '}}},
            {'j', {{L' ', L'▝', L' '}, {L' ', L'▐', L' '}, {L'▝', L'▞', L' '}}},
            {'k', {{L'▌', L' ', L' '}, {L'▙', L'▄', L'▘'}, {L'▌', L' ', L'▌'}}},
            {'l', {{L' ', L'▌', L' '}, {L' ', L'▌', L' '}, {L' ', L'▚', L' '}}},
            {'m', {{L' ', L' ', L' '}, {L'▛', L'▞', L'▖'}, {L'▌', L'▌', L'▌'}}},
            {'n', {{L' ', L' ', L' '}, {L'▛', L'▀', L'▚'}, {L'▌', L' ', L'▐'}}},
            {'o', {{L' ', L' ', L' '}, {L'▞', L'▀', L'▚'}, {L'▚', L'▄', L'▞'}}},
            {'p', {{L'▞', L'▀', L'▚'}, {L'▙', L'▄', L'▞'}, {L'▌', L' ', L' '}}},
            {'q', {{L'▞', L'▀', L'▚'}, {L'▚', L'▄', L'▟'}, {L' ', L' ', L'▐'}}},
            {'r', {{L' ', L' ', L' '}, {L'▙', L'▀', L'▚'}, {L'▌', L' ', L' '}}},
            {'s', {{L'▗', L'▄', L'▖'}, {L'▚', L'▄', L'▖'}, {L'▗', L'▄', L'▞'}}},
            {'t', {{L' ', L'▌', L' '}, {L'▀', L'▛', L'▀'}, {L' ', L'▚', L' '}}},
            {'u', {{L' ', L' ', L' '}, {L'▌', L' ', L'▐'}, {L'▚', L'▄', L'▟'}}},
            {'v', {{L' ', L' ', L' '}, {L'▌', L' ', L'▐'}, {L'▝', L'▄', L'▘'}}},
            {'w', {{L' ', L' ', L' '}, {L'▐', L'▐', L'▐'}, {L'▝', L'▞', L'▟'}}},
            {'x', {{L' ', L' ', L' '}, {L'▝', L'▄', L'▘'}, {L'▗', L'▀', L'▖'}}},
            {'y', {{L'▌', L' ', L'▐'}, {L'▚', L'▄', L'▟'}, {L'▗', L'▄', L'▞'}}},
            {'z', {{L'▄', L'▄', L'▄'}, {L'▗', L'▄', L'▞'}, {L'▙', L'▄', L'▄'}}},
            {'A', {{L'▞', L'▀', L'▚'}, {L'▙', L'▄', L'▟'}, {L'▌', L' ', L'▐'}}},
            {'B', {{L'▛', L'▀', L'▚'}, {L'▛', L'▀', L'▚'}, {L'▙', L'▄', L'▞'}}},
            {'C', {{L'▞', L'▀', L'▚'}, {L'▌', L' ', L' '}, {L'▚', L'▄', L'▞'}}},
            {'D', {{L'▛', L'▀', L'▚'}, {L'▌', L' ', L'▐'}, {L'▙', L'▄', L'▞'}}},
            {'E', {{L'▛', L'▀', L'▀'}, {L'▛', L'▀', L'▀'}, {L'▙', L'▄', L'▄'}}},
            {'F', {{L'▛', L'▀', L'▀'}, {L'▛', L'▀', L'▀'}, {L'▌', L' ', L' '}}},
            {'G', {{L'▞', L'▀', L'▚'}, {L'▌', L' ', L'▄'}, {L'▚', L'▄', L'▟'}}},
            {'H', {{L'▌', L' ', L'▐'}, {L'▛', L'▀', L'▜'}, {L'▌', L' ', L'▐'}}},
            {'I', {{L'▀', L'▜', L'▀'}, {L' ', L'▐', L' '}, {L'▄', L'▟', L'▄'}}},
            {'J', {{L'▀', L'▜', L'▀'}, {L' ', L'▐', L' '}, {L'▚', L'▟', L' '}}},
            {'K', {{L'▌', L' ', L'▞'}, {L'▛', L'▀', L'▖'}, {L'▌', L' ', L'▐'}}},
            {'L', {{L'▌', L' ', L' '}, {L'▌', L' ', L' '}, {L'▙', L'▄', L'▄'}}},
            {'M', {{L'▙', L' ', L'▟'}, {L'▌', L'▀', L'▐'}, {L'▌', L' ', L'▐'}}},
            {'N', {{L'▙', L' ', L'▐'}, {L'▌', L'▚', L'▐'}, {L'▌', L' ', L'▜'}}},
            {'O', {{L'▞', L'▀', L'▚'}, {L'▌', L' ', L'▐'}, {L'▚', L'▄', L'▞'}}},
            {'P', {{L'▛', L'▀', L'▚'}, {L'▙', L'▄', L'▞'}, {L'▌', L' ', L' '}}},
            {'Q', {{L'▞', L'▀', L'▚'}, {L'▌', L'▗', L'▐'}, {L'▚', L'▄', L'▚'}}},
            {'R', {{L'▛', L'▀', L'▚'}, {L'▙', L'▄', L'▞'}, {L'▌', L' ', L'▐'}}},
            {'S', {{L'▞', L'▀', L'▘'}, {L'▝', L'▀', L'▚'}, {L'▚', L'▄', L'▞'}}},
            {'T', {{L'▀', L'▜', L'▀'}, {L' ', L'▐', L' '}, {L' ', L'▐', L' '}}},
            {'U', {{L'▌', L' ', L'▐'}, {L'▌', L' ', L'▐'}, {L'▚', L'▄', L'▞'}}},
            {'V', {{L'▌', L' ', L'▐'}, {L'▚', L' ', L'▞'}, {L'▝', L'▄', L'▘'}}},
            {'W', {{L'▌', L' ', L'▐'}, {L'▌', L'▄', L'▐'}, {L'▛', L' ', L'▜'}}},
            {'X', {{L'▚', L' ', L'▞'}, {L' ', L'█', L' '}, {L'▞', L' ', L'▚'}}},
            {'Y', {{L'▌', L' ', L'▐'}, {L'▝', L'▄', L'▘'}, {L' ', L'█', L' '}}},
            {'Z', {{L'▀', L'▀', L'▜'}, {L'▗', L'▞', L'▘'}, {L'▙', L'▄', L'▄'}}},
            {'0', {{L'▞', L'▀', L'▙'}, {L'▌', L'▞', L'▐'}, {L'▜', L'▄', L'▞'}}},
            {'1', {{L' ', L'▟', L' '}, {L' ', L'▐', L' '}, {L' ', L'▟', L'▖'}}},
            {'2', {{L'▞', L'▀', L'▚'}, {L' ', L'▗', L'▞'}, {L'▟', L'▙', L'▄'}}},
            {'3', {{L'▞', L'▀', L'▚'}, {L' ', L'▀', L'▚'}, {L'▚', L'▄', L'▞'}}},
            {'4', {{L'▌', L' ', L'▌'}, {L'▝', L'▀', L'▛'}, {L' ', L' ', L'▌'}}},
            {'5', {{L'▛', L'▀', L'▀'}, {L'▀', L'▀', L'▚'}, {L'▚', L'▄', L'▞'}}},
            {'6', {{L'▞', L'▀', L'▀'}, {L'▛', L'▀', L'▚'}, {L'▚', L'▄', L'▞'}}},
            {'7', {{L'▀', L'▀', L'▜'}, {L' ', L'▗', L'▘'}, {L' ', L'▌', L' '}}},
            {'8', {{L'▞', L'▀', L'▚'}, {L'▞', L'▀', L'▚'}, {L'▚', L'▄', L'▞'}}},
            {'9', {{L'▞', L'▀', L'▚'}, {L'▚', L'▄', L'▟'}, {L'▗', L'▄', L'▟'}}},
            {'`', {{L'▗', L' ', L' '}, {L' ', L'▘', L' '}, {L' ', L' ', L' '}}},
            {'~', {{L'▗', L'▖', L'▗'}, {L'▘', L'▝', L'▘'}, {L' ', L' ', L' '}}},
            {'!', {{L' ', L'█', L' '}, {L' ', L'█', L' '}, {L' ', L'▄', L' '}}},
            {'@', {{L'▞', L'▀', L'▚'}, {L'▌', L'█', L'▟'}, {L'▚', L'▄', L'▄'}}},
            {'#', {{L'▟', L'▄', L'▙'}, {L'▐', L' ', L'▌'}, {L'▜', L'▀', L'▛'}}},
            {'$', {{L'▗', L'▙', L'▖'}, {L'▚', L'▙', L'▖'}, {L'▗', L'▙', L'▞'}}},
            {'%', {{L'█', L' ', L'▞'}, {L' ', L'▞', L' '}, {L'▞', L' ', L'█'}}},
            {'^', {{L' ', L'▄', L' '}, {L'▝', L' ', L'▘'}, {L' ', L' ', L' '}}},
            {'&', {{L'▞', L'▚', L' '}, {L'▞', L'▌', L'▖'}, {L'▚', L'▞', L'▖'}}},
            {'*', {{L'▝', L'▄', L'▘'}, {L'▗', L'▀', L'▖'}, {L' ', L' ', L' '}}},
            {'(', {{L' ', L'▞', L'▘'}, {L'▐', L' ', L' '}, {L' ', L'▚', L'▖'}}},
            {')', {{L'▝', L'▚', L' '}, {L' ', L' ', L'▌'}, {L'▗', L'▞', L' '}}},
            {'-', {{L' ', L' ', L' '}, {L'▗', L'▄', L'▖'}, {L' ', L' ', L' '}}},
            {'_', {{L' ', L' ', L' '}, {L' ', L' ', L' '}, {L'▄', L'▄', L'▄'}}},
            {'=', {{L' ', L' ', L' '}, {L'▄', L'█', L'▄'}, {L' ', L'▀', L' '}}},
            {'+', {{L' ', L' ', L' '}, {L'▝', L'▀', L'▘'}, {L'▝', L'▀', L'▘'}}},
            {'[', {{L'▐', L'▀', L'▘'}, {L'▐', L' ', L' '}, {L'▐', L'▄', L'▖'}}},
            {'{', {{L' ', L'▛', L'▘'}, {L'█', L' ', L' '}, {L' ', L'▙', L'▖'}}},
            {']', {{L'▝', L'▀', L'▌'}, {L' ', L' ', L'▌'}, {L'▗', L'▄', L'▌'}}},
            {'}', {{L'▝', L'▜', L' '}, {L' ', L' ', L'█'}, {L'▗', L'▟', L' '}}},
            {'\\', {{L' ', L'█', L' '}, {L' ', L'█', L' '}, {L' ', L'█', L' '}}},
            {'|', {{L'▚', L' ', L' '}, {L' ', L'▚', L' '}, {L' ', L' ', L'▚'}}},
            {';', {{L' ', L' ', L' '}, {L' ', L'▀', L' '}, {L' ', L'▜', L' '}}},
            {':', {{L' ', L' ', L' '}, {L' ', L'▀', L' '}, {L' ', L'▄', L' '}}},
            {'\'', {{L' ', L'▗', L'▖'}, {L' ', L' ', L'▘'}, {L' ', L' ', L' '}}},
            {'"', {{L'▗', L' ', L'▖'}, {L'▝', L' ', L'▘'}, {L' ', L' ', L' '}}},
            {',', {{L' ', L' ', L' '}, {L' ', L' ', L' '}, {L' ', L'▜', L' '}}},
            {'<', {{L' ', L'▗', L'▖'}, {L'▐', L'▌', L' '}, {L' ', L'▝', L'▘'}}},
            {'.', {{L' ', L' ', L' '}, {L' ', L' ', L' '}, {L' ', L'▄', L' '}}},
            {'>', {{L'▗', L'▖', L' '}, {L' ', L'▐', L'▌'}, {L'▝', L'▘', L' '}}},
            {'?', {{L'▞', L'▀', L'▚'}, {L' ', L'▄', L'▘'}, {L' ', L'▄', L' '}}},
            {'/', {{L' ', L' ', L'▞'}, {L' ', L'▞', L' '}, {L'▞', L' ', L' '}}},
        };
    } Keys;

    /// @brief Initialize - Acts as the ncursespp version of initscr() with a few other initializations; end() must be called at the end of a program
    /// @param useMouse Whether to set up the ability to take mouse inputs or not
    /// @returns True or false for whether the user's terminal will have full color support
    bool init(bool useMouse = false);

    /// @brief End - Acts as the ncursespp version of endwin() with some extra cleanup
    /// @param useMouse Whether to clean up the mouse settings or not
    /// @param funcReturn An integer for the function to return (especially useful for reducing clutter in a main() function or smth)
    /// @returns funcReturn
    int end(bool useMouse = false, int funcReturn = 0);
}
