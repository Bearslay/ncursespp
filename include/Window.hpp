#pragma once

#include "General.hpp"
#include "Mouse.hpp"

namespace npp {
    /// @brief The npp version of the WINDOW class from ncurses.h - comes with better support for unicode characters, much better line drawing capabilities, flashy rendering animations, and other fun bonuses
    class Window {
        private:
            /// @brief ncurses window that allows everything to be interacted with inside a compatible terminal
            WINDOW *Win;

            /// @brief Y-dimension (rows) of the window
            unsigned short DimY;
            /// @brief X-dimension (cols) of the window
            unsigned short DimX;
            /// @brief Y-position (row) of the top-left corner of the window
            unsigned short PosY;
            /// @brief X-position (col) of the top-left corner of the window
            unsigned short PosX;

            /// @brief Amount of characters to add padding to from the top of the screen
            unsigned short PadUp = 0;
            /// @brief Amount of characters to add padding to from the bottom of the screen
            unsigned short PadDown = 0;
            /// @brief Amount of characters to add padding to from the left of the screen
            unsigned short PadLeft = 0;
            /// @brief Amount of characters to add padding to from the right of the screen
            unsigned short PadRight = 0;

            /// @brief If the user can skip wait() functions with an input
            bool CanSkip = true;

            /// @brief Contain the data (character/color/attributes) for each cell
            struct Cell {
                /// @brief Character contained in the cell
                wchar_t Char = L' ';

                /// @brief Color pair used for the cell
                unsigned char Color = 1;

                /// @brief Whether the cell is bolded or not
                bool Bold = false;
                /// @brief Whether the cell is italicized or not
                bool Italic = false;
                /// @brief Whether the cell is underlined or not
                bool Under = false;
                /// @brief Whether the cell is reversed (color pair) or not
                bool Rev = false;
                /// @brief Whether the cell blinks or not
                bool Blink = false;
                /// @brief Whether the cell is dim or not
                bool Dim = false;
                /// @brief Whether the cell is invisible or not
                bool Invis = false;
                /// @brief Whether the cell should stand out or not
                bool Stand = false;
                /// @brief Whether the cell should be protected or not
                bool Prot = false;
                /// @brief Whether the cell should use the alternate character set or not
                bool Alt = false;

                /// @brief If the cell is a line drawing character, whether it can be merged with or not
                bool CanMerge = false;
            };
            std::vector<std::vector<Cell>> Grid;

            //
            // INTERFACING WITH NCURSES
            //

            /// @brief Write - Write a cell to the ncurses window
            /// @param y Y-position (row) of the cell to write
            /// @param x X-position (col) of the cell to write
            void write(unsigned short y, unsigned short x);

            /// @brief Extract Attributes - Extract a string input into a set of booleans
            /// @param input Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @returns A vector (size of 10) of booleans representing attributes
            std::vector<bool> extractAttributes(std::string input);
            /// @brief Toggle Cell Attributes - Toggle the attributes being used when writing to a cell - boolean based (acts as wattron() or wattroff())
            /// @param bold Bold
            /// @param italic Italic
            /// @param under Underline
            /// @param rev Reverse
            /// @param blink Blink
            /// @param dim Dim
            /// @param invis Invisible
            /// @param stand Standout
            /// @param prot Protected
            /// @param alt Alt Char Set
            void toggleAttributes(bool bold, bool italic, bool under, bool rev, bool blink, bool dim, bool invis, bool stand, bool prot, bool alt);
            /// @brief Toggle Cell Attributes - Toggle the attributes being used when writing to a cell - string based (acts as wattron() or wattroff())
            /// @param input A string that should consist of two-letter keys for which attributes to toggle
            void toggleAttributes(std::string input);

            //
            // TIMING
            //

            /// @brief Wait - Pause everything for an amount of milliseconds - base
            /// @param millis Milliseconds to wait for
            /// @param skippable Whether the wait can be skipped (with a keyboard input) or not
            /// @returns True if the wait was skipped, false if the wait wasn't
            bool wait(unsigned long millis);

            //
            // COMMON CHECKS & CODE SHORTCUTS
            //

            /// @brief Check Coordinate - Check if the coordinates are within the window or not
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) of the coordinate to check
            /// @returns True if the coordinates are in the window, false if the coordinates aren't
            bool checkCoord(std::pair<unsigned short, unsigned short> pos);
            /// @brief Check Coordinate - Check if the coordinates are within the window or not
            /// @param y y-position (row) of the coordinate to check
            /// @param x x-position (col) of the coordinate to check
            /// @returns True if the coordinates are in the window, false if the coordinates aren't
            bool checkCoord(unsigned short y, unsigned short x);

            //
            // LINE DRAWING HELPERS
            //

            /// @brief Get Connect Style - Get a number corresponding to the type of line that would be needed to connect to the specified character in the specified direction
            /// @param y y-position (row) of the character to get a connection style from
            /// @param x x-position (col) of the character to get a connection style from
            /// @param dir The direction to check: 0 = Up/North, 1 = Down/South, 2 = Left/West, 3 = Right/East
            /// @returns A number 0-3: 0 = Nothing, 1 = Light, 2 = Heavy, 3 = Double
            const unsigned char getConnectStyle(unsigned short y, unsigned short x, unsigned char dir);

            /// @brief Get Piece - Convert a set of directions (connection styles) and a line style into the corresponding unicode character
            /// @param dir A set of four directions (up, down, left, right)
            /// @param style A pair consisting of a number corresponding to the main style (0 = Light Hard, 1 = Light Soft, 2 = Heavy Both, 3 = Heavy Horizontal, 4 = Heavy Vertical, 5 = Doubled Both, 6 = Doubled Horizontal, 7 = Doubled Vertical) and a number corresponding to the dash style (0 = None, 1 = Double, 2 = Triple, 3 = Quardruple, 4 = Single 1, 5 = Single 2)
            /// @returns A unicode character (box drawing) if a match to the directions is found, a space if a match isn't found
            const wchar_t getPiece(std::vector<unsigned char> dir, std::pair<unsigned char, unsigned char> style);

        public:
            /// @brief Create an ncursespp Window
            /// @param y Y position (row) for the top-left corner of the Window
            /// @param x X position (col) for the top-left corner of the Window
            /// @param dimy Height (rows) of the Window
            /// @param dimx Length (cols) of the Window
            Window(unsigned short y = 0, unsigned short x = 0, unsigned short dimy = LINES, unsigned short dimx = COLS);
            /// @brief Create an auto-centered Window relative to the screen
            /// @param dimy Height (rows) of the Window
            /// @param dimx Length (cols) of the Window
            Window(unsigned short dimy, unsigned short dimx);
            /// @brief Create an auto-centered Window relative to another window
            /// @param w Another window to center the new one to
            /// @param dimy Height (rows) of the Window
            /// @param dimx Length (cols) of the Window
            Window(Window &win, unsigned short dimy, unsigned short dimx);

            //
            // MISC
            //

            /// @brief Return Wait - Pause everything for an amount of milliseconds - char return
            /// @param millis Milliseconds to wait for
            /// @returns An integer that can be used in the same way that it is in the ncurses getch() function
            int rwait(unsigned long millis);
            /// @brief Target Wait - Pause everything for an amount of milliseconds - target
            /// @param millis Milliseconds to wait for
            /// @param targets A list of characters that would skip the wait if detected
            /// @returns True if the wait was skipped, false if the wait wasn't
            bool twait(unsigned long millis, std::vector<char> targets);

            /// @brief Update Skippability - Change whether the window will allow the user to skip wait() functions
            /// @param skippable Whether the window will allow the user to skip or not
            void uskip(bool skippable);

            /// @brief Update Padding (Reset) - Reset the window's padding (sets all padding to zero)
            void upreset();
            /// @brief Update Padding (Top) - Change how many padding spaces will be on the top of the window
            /// @param padding Amount of characters to pad
            void upup(unsigned short padding = 0);
            /// @brief Update Padding (Bottom) - Change how many padding spaces will be on the bottom of the window
            /// @param padding Amount of characters to pad
            void updown(unsigned short padding = 0);
            /// @brief Update Padding (Left) - Change how many padding spaces will be on the left of the window
            /// @param padding Amount of characters to pad
            void upleft(unsigned short padding = 0);
            /// @brief Update Padding (Right) - Change how many padding spaces will be on the right of the window
            /// @param padding Amount of characters to pad
            void upright(unsigned short padding = 0);
            /// @brief Update Padding (All) - Change how many padding spaces will be on all sides of the window
            /// @param padding 
            void upall(unsigned short padding = 0);

            //
            // GETTING WINDOW/CELL ATTRIBUTES
            //

            /// @brief Get Y-Dimension - Get the y-dimension (rows) of a window
            /// @returns The y-dimension (rows) of a window
            const unsigned short gdimy();
            /// @brief Get X-Dimension - Get the x-dimension (cols) of a window
            /// @returns The x-dimension (cols) of a window
            const unsigned short gdimx();
            /// @brief Get Y-Position - Get the y-position (row) of the top-left corner of a window
            /// @returns The y-dimension (row) of the top-left corner of a window
            const unsigned short gposy();
            /// @brief Get X-Position - Get the x-position (col) of the top-left corner of a window
            /// @returns The x-dimension (col) of the top-left corner of a window
            const unsigned short gposx();
            /// @brief Get Padding (Top) - Get the amount of characters padding the top of the screen
            /// @returns The amount of characters padding the top of the screen
            const unsigned short gpadt();
            /// @brief Get Padding (Bottom) - Get the amount of characters padding the bottom of the screen
            /// @returns The amount of characters padding the bottom of the screen
            const unsigned short gpadb();
            /// @brief Get Padding (Left) - Get the amount of characters padding the left of the screen
            /// @returns The amount of characters padding the left of the screen
            const unsigned short gpadl();
            /// @brief Get Padding (Right) - Get the amount of characters padding the right of the screen
            /// @returns The amount of characters padding the right of the screen
            const unsigned short gpadr();

            /// @brief Scan Character - Get the character of a cell from a window
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A string (length of 1) representing a cell's character or a space if the requested cell doesn't exist
            const wchar_t schar(unsigned short y, unsigned short x);
            /// @brief Scan Color - Get the color pair of a cell from a window
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns The number of the color pair a cell uses or the default pair for the window if the requested cell doesn't exist
            const unsigned char scolor(unsigned short y, unsigned short x);
            /// @brief Scan Bold - Get whether a cell from a window is bolded or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is bolded or the default state for the window if the requested cell doesn't exist
            const bool sbold(unsigned short y, unsigned short x);
            /// @brief Scan Italic - Get whether a cell from a window is italicized or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is italicized or the default state for the window if the requested cell doesn't exist
            const bool sitalic(unsigned short y, unsigned short x);
            /// @brief Scan Underline - Get whether a cell from a window is underlined or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is underlined or the default state for the window if the requested cell doesn't exist
            const bool sunder(unsigned short y, unsigned short x);
            /// @brief Scan Reverse - Get whether a cell from a window is reversed or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is reversed or the default state for the window if the requested cell doesn't exist
            const bool srev(unsigned short y, unsigned short x);
            /// @brief Scan Blink - Get whether a cell from a window is blinking or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is blinking or the default state for the window if the requested cell doesn't exist
            const bool sblink(unsigned short y, unsigned short x);
            /// @brief Scan Dim - Get whether a cell from a window is dim or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is dim or the default state for the window if the requested cell doesn't exist
            const bool sdim(unsigned short y, unsigned short x);
            /// @brief Scan Invisible - Get whether a cell from a window is invisible or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is invisible or the default state for the window if the requested cell doesn't exist
            const bool sinvis(unsigned short y, unsigned short x);
            /// @brief Scan Standout - Get whether a cell from a window is standing out or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is standing out or the default state for the window if the requested cell doesn't exist
            const bool sstand(unsigned short y, unsigned short x);
            /// @brief Scan Protected - Get whether a cell from a window is protected or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is protected or the default state for the window if the requested cell doesn't exist
            const bool sprot(unsigned short y, unsigned short x);
            /// @brief Scan Alt Char Set - Get whether a cell from a window is using the alternate character set or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is using the alternate character set or the default state for the window if the requested cell doesn't exist
            const bool salt(unsigned short y, unsigned short x);
            /// @brief Scan Mergeability - Get whether a cell can merge with other cells (should only apply to cells that contain box drawing characters)
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell can merge with other cells
            const bool smerge(unsigned short y, unsigned short x);

            //
            // WRITING TO WINDOW
            //

            /// @brief Clear the window without removing cell data
            void clear();
            /// @brief Clear the window and remove cell data
            void reset();

            /// @brief Write Character, Return Position - Write a single character to the window - pair pos, pair return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) for the character to be written at
            /// @param input Wide character input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @returns A pair consisting of a y-position (row) and an x-position (col) based off of the character's position and the inputted offset
            std::pair<unsigned short, unsigned short> wcharp(std::pair<unsigned short, unsigned short> pos, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset);
            /// @brief Write Character, Return Position - Write a single character (wchar_t) to the window - short short pos, pair return
            /// @param y y-position (row) of the character
            /// @param x x-position (col) of the character
            /// @param input Wide character input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @returns A pair consisting of a y-position (row) and an x-position (col) based off of the character's position and the inputted offset
            std::pair<unsigned short, unsigned short> wcharp(unsigned short y, unsigned short x, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset);
            /// @brief Write Character, Return Y-Position - Write a single character to the window - pair pos, short return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) for the character to be written at
            /// @param input Wide character input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @returns A y-position (row) based off of the character's position and the inputted offset
            unsigned short wchary(std::pair<unsigned short, unsigned short> pos, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset);
            /// @brief Write Character, Return Y-Position - Write a single character to the window - short short pos, short return
            /// @param y y-position (row) of the character
            /// @param x x-position (col) of the character
            /// @param input Wide character input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @returns A y-position (row) based off of the character's position and the inputted offset
            unsigned short wchary(unsigned short y, unsigned short x, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset);
            /// @brief Write Character, Return X-Position - Write a single character to the window - pair pos, short return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) for the character to be written at
            /// @param input Wide character input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @returns A x-position (col) based off of the character's position and the inputted offset
            unsigned short wcharx(std::pair<unsigned short, unsigned short> pos, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset);
            /// @brief Write Character, Return X-Position - Write a single character to the window - short short pos, short return
            /// @param y y-position (row) of the character
            /// @param x x-position (col) of the character
            /// @param input Wide character input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @returns A x-position (col) based off of the character's position and the inputted offset
            unsigned short wcharx(unsigned short y, unsigned short x, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset);
            /// @brief Write Character, Return Nothing - Write a single character to the window - pair pos, no return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) for the character to be written at
            /// @param input Wide character input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            void wchar(std::pair<unsigned short, unsigned short> pos, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes);
            /// @brief Write Character, Return Nothing - Write a single character to the window - short short pos, no return
            /// @param y y-position (row) of the character
            /// @param x x-position (col) of the character
            /// @param input Wide character input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            void wchar(unsigned short y, unsigned short x, wchar_t input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes);

            /// @brief Write String, Return Position - Write a string to the window - pair pos, pair return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) indicating the start of the string
            /// @param input String input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A pair consisting of a y-position (row) and an x-position (col) based off of the last character of the string that was written and the inputted offset
            std::pair<unsigned short, unsigned short> wstrp(std::pair<unsigned short, unsigned short> pos, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write String, Return Position - Write a string to the window - short short pos, pair return
            /// @param y y-position (row) of the start of the string
            /// @param x x-position (col) of the start of the string
            /// @param input String input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A pair consisting of a y-position (row) and an x-position (col) based off of the last character of the string that was written and the inputted offset
            std::pair<unsigned short, unsigned short> wstrp(unsigned short y, unsigned short x, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write String, Return Y-Position - Write a string to the window - pair pos, short return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) indicating the start of the string
            /// @param input String input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A y-position (row) based off of the last character of the string that was written and the inputted offset
            unsigned short wstry(std::pair<unsigned short, unsigned short> pos, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write String, Return Y-Position - Write a string to the window - short short pos, short return
            /// @param y y-position (row) of the start of the string
            /// @param x x-position (col) of the start of the string
            /// @param input String input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A y-position (row) based off of the last character of the string that was written and the inputted offset
            unsigned short wstry(unsigned short y, unsigned short x, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write String, Return X-Position - Write a string to the window - pair pos, short return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) indicating the start of the string
            /// @param input String input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A x-position (col) based off of the last character of the string that was written and the inputted offset
            unsigned short wstrx(std::pair<unsigned short, unsigned short> pos, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write String, Return X-Position - Write a string to the window - short short pos, short return
            /// @param y y-position (row) of the start of the string
            /// @param x x-position (col) of the start of the string
            /// @param input String input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A x-position (col) based off of the last character of the string that was written and the inputted offset
            unsigned short wstrx(unsigned short y, unsigned short x, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write String, Return Nothing - Write a string to the window - pair pos, no return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) indicating the start of the string
            /// @param input String input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param wrap Whether to wrap to starting x or not
            void wstr(std::pair<unsigned short, unsigned short> pos, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, bool wrap = Defaults.Wrap);
            /// @brief Write String, Return Nothing - Write a string to the window - short short pos, no return
            /// @param y y-position (row) of the start of the string
            /// @param x x-position (col) of the start of the string
            /// @param input String input to be written (unicode friendly)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param wrap Whether to wrap to starting x or not
            void wstr(unsigned short y, unsigned short x, std::wstring input, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, bool wrap = Defaults.Wrap);

            /// @brief Write Integer, Return Position - Write an integer to the window - pair pos, pair return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) indicating the start of the integer
            /// @param input Integer input to be written
            /// @param minWidth Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A pair consisting of a y-position (row) and x-position (col) of the last character of the integer that was written
            std::pair<unsigned short, unsigned short> wintp(std::pair<unsigned short, unsigned short> pos, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write Integer, Return Position - Write an integer to the window - short short pos, pair return
            /// @param y y-position (row) of the start of the integer
            /// @param x x-position (col) of the start of the integer
            /// @param input Integer input to be written
            /// @param minWidth Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A pair consisting of the y-position (row) and x-position (col) of the last character of the integer that was written
            std::pair<unsigned short, unsigned short> wintp(unsigned short y, unsigned short x, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write Integer, Return Y-Position - Write an integer to the window - pair pos, short return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) indicating the start of the integer
            /// @param input Integer input to be written
            /// @param minWidth Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A y-position (row) based off of the last character of the integer that was written and the inputted offset
            unsigned short winty(std::pair<unsigned short, unsigned short> pos, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write Integer, Return Y-Position - Write an integer to the window - short short pos, short return
            /// @param y y-position (row) of the start of the integer
            /// @param x x-position (col) of the start of the integer
            /// @param input Integer input to be written
            /// @param minWidth Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A y-position (row) based off of the last character of the integer that was written and the inputted offset
            unsigned short winty(unsigned short y, unsigned short x, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write Integer, Return X-Position - Write an integer to the window - pair pos, short return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) indicating the start of the integer
            /// @param input Integer input to be written
            /// @param minWidth Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A x-position (col) based off of the last character of the integer that was written and the inputted offset
            unsigned short wintx(std::pair<unsigned short, unsigned short> pos, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write Integer, Return X-Position - Write an integer to the window - short short pos, short return
            /// @param y y-position (row) of the start of the integer
            /// @param x x-position (col) of the start of the integer
            /// @param input Integer input to be written
            /// @param minWidth Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A x-position (col) based off of the last character of the integer that was written and the inputted offset
            unsigned short wintx(unsigned short y, unsigned short x, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write Integer, Return Nothing - Write an integer to the window - pair pos, no return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) indicating the start of the integer
            /// @param input Integer input to be written
            /// @param minWidth Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param wrap Whether to wrap to starting x or not
            void wint(std::pair<unsigned short, unsigned short> pos, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, bool wrap = Defaults.Wrap);
            /// @brief Write Integer, Return Nothing - Write an integer to the window - short short pos, no return
            /// @param y y-position (row) of the start of the integer
            /// @param x x-position (col) of the start of the integer
            /// @param input Integer input to be written
            /// @param minWidth Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param color Color pair to use when writing each cell
            /// @param att Set of attributes to apply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param wrap Whether to wrap to starting x or not
            void wint(unsigned short y, unsigned short x, int input, unsigned short minWidth = Defaults.MinWidth, unsigned char color = Defaults.Color, std::string att = Defaults.Attributes, bool wrap = Defaults.Wrap);

            /// @brief Write Matrix String, Return Position - Write a string to the window using the 6x6 matrix text - pair pos, pair return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) indicating the top-left corner of the first character of the string
            /// @param input String input to be written (no unicode, somewhat limited character set)
            /// @param color Color pair to use when writing each cell
            /// @param danglingLetters Whether to allow certain letters to hang beneath the usual line (increases leading by 1)
            /// @param kerning The amount of spaces between each letter
            /// @param leading The amount of spaces between each line
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A pair consisting of a y-position (row) and x-position (col) of the bottom-right corner of the last character of the text that was written
            std::pair<unsigned short, unsigned short> wmstrp(std::pair<unsigned short, unsigned short> pos, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write Matrix String, Return Position - Write a string to the window using the 6x6 matrix text - short short pos, pair return
            /// @param y y-position (row) indicating the top-left corner of the first character of the string
            /// @param x x-position (col) indicating the top-left corner of the first character of the string
            /// @param input String input to be written (no unicode, somewhat limited character set)
            /// @param color Color pair to use when writing each cell
            /// @param danglingLetters Whether to allow certain letters to hang beneath the usual line (increases leading by 1)
            /// @param kerning The amount of spaces between each letter
            /// @param leading The amount of spaces between each line
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns A pair consisting of a y-position (row) and x-position (col) of the bottom-right corner of the last character of the text that was written
            std::pair<unsigned short, unsigned short> wmstrp(unsigned short y, unsigned short x, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write Matrix String, Return Y-Position - Write a string to the window using the 6x6 matrix text - pair pos, short return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) indicating the top-left corner of the first character of the string
            /// @param input String input to be written (no unicode, somewhat limited character set)
            /// @param color Color pair to use when writing each cell
            /// @param danglingLetters Whether to allow certain letters to hang beneath the usual line (increases leading by 1)
            /// @param kerning The amount of spaces between each letter
            /// @param leading The amount of spaces between each line
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns The y-position (row) of the bottom-right corner of the last character of the text that was written
            unsigned short wmstry(std::pair<unsigned short, unsigned short> pos, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write Matrix String, Return Y-Position - Write a string to the window using the 6x6 matrix text - short short pos, short return
            /// @param y y-position (row) indicating the top-left corner of the first character of the string
            /// @param x x-position (col) indicating the top-left corner of the first character of the string
            /// @param input String input to be written (no unicode, somewhat limited character set)
            /// @param color Color pair to use when writing each cell
            /// @param danglingLetters Whether to allow certain letters to hang beneath the usual line (increases leading by 1)
            /// @param kerning The amount of spaces between each letter
            /// @param leading The amount of spaces between each line
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns The y-position (row) of the bottom-right corner of the last character of the text that was written
            unsigned short wmstry(unsigned short y, unsigned short x, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write Matrix String, Return X-Position - Write a string to the window using the 6x6 matrix text - pair pos, short return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) indicating the top-left corner of the first character of the string
            /// @param input String input to be written (no unicode, somewhat limited character set)
            /// @param color Color pair to use when writing each cell
            /// @param danglingLetters Whether to allow certain letters to hang beneath the usual line (increases leading by 1)
            /// @param kerning The amount of spaces between each letter
            /// @param leading The amount of spaces between each line
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns The x-position (col) of the bottom-right corner of the last character of the text that was written
            unsigned short wmstrx(std::pair<unsigned short, unsigned short> pos, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write Matrix String, Return X-Position - Write a string to the window using the 6x6 matrix text - short short pos, short return
            /// @param y y-position (row) indicating the top-left corner of the first character of the string
            /// @param x x-position (col) indicating the top-left corner of the first character of the string
            /// @param input String input to be written (no unicode, somewhat limited character set)
            /// @param color Color pair to use when writing each cell
            /// @param danglingLetters Whether to allow certain letters to hang beneath the usual line (increases leading by 1)
            /// @param kerning The amount of spaces between each letter
            /// @param leading The amount of spaces between each line
            /// @param offset Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param wrap Whether to wrap to starting x or not
            /// @returns The x-position (col) of the bottom-right corner of the last character of the text that was written
            unsigned short wmstrx(unsigned short y, unsigned short x, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, std::pair<unsigned short, unsigned short> offset = Defaults.Offset, bool wrap = Defaults.Wrap);
            /// @brief Write Matrix String, Return Nothing - Write a string to the window using the 6x6 matrix text - pair pos, no return
            /// @param pos Pair consisting of a y-position (row) and an x-position (col) indicating the top-left corner of the first character of the string
            /// @param input String input to be written (no unicode, somewhat limited character set)
            /// @param color Color pair to use when writing each cell
            /// @param danglingLetters Whether to allow certain letters to hang beneath the usual line (increases leading by 1)
            /// @param kerning The amount of spaces between each letter
            /// @param leading The amount of spaces between each line
            /// @param wrap Whether to wrap to starting x or not
            void wmstr(std::pair<unsigned short, unsigned short> pos, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, bool wrap = Defaults.Wrap);
            /// @brief Write Matrix String, Return Nothing - Write a string to the window using the 6x6 matrix text - short short pos, no return
            /// @param y y-position (row) indicating the top-left corner of the first character of the string
            /// @param x x-position (col) indicating the top-left corner of hte first character of the string
            /// @param input String input to be written (no unicode, somewhat limited character set)
            /// @param color Color pair to use when writing each cell
            /// @param danglingLetters Whether to allow certain letters to hang beneath the usual line (increases leading by 1)
            /// @param kerning The amount of spaces between each letter
            /// @param leading The amount of spaces between each line
            /// @param wrap Whether to wrap to starting x or not
            void wmstr(unsigned short y, unsigned short x, std::string input, unsigned char color = Defaults.Color, bool danglingLetters = Defaults.DanglingLetters, unsigned char kerning = Defaults.Kerning, unsigned char leading = Defaults.Leading, bool wrap = Defaults.Wrap);

            //
            // GET USER INPUT
            //

            /// @brief Get Character - Get a single character input from the user (acts as the ncursespp version of wgetch())
            /// @param enableKeypad Allow the use of arrow keys and such
            /// @param pause Pause the program until an input is read
            /// @param autoRender Automatically render the window when the function is called
            /// @returns An integer that can be used in the same way that it is in the ncurses getch() function
            int gchar(bool pause = true, bool enableKeypad = true, bool autoRender = true);

            /// @brief Get String - Get a string input from the user (acts as the ncursespp version of mvwgetnstr())
            /// @param y y-position (row) of the start of the input location
            /// @param x x-position (col) of the start of the input location
            /// @param maxChars Maximum amount of characters the string can contain
            /// @param echoColor Color pair to use when echoing the user's input
            /// @param echoAtt Set of attributes to apply to the echoed string
            /// @param autoWrite Whether to automatically write the inputted string to the window
            /// @param showStr Whether to echo the user's input or not
            /// @param showCursor Whether to show the cursor when taking an input or not
            /// @param enableKeypad Whether to allow the use of the keypad when taking the input
            /// @returns A wide string (not unicode friendly :/)
            std::wstring gstr(unsigned short y, unsigned short x, int maxChars = 255, unsigned char echoColor = Defaults.Color, std::string echoAtt = Defaults.Attributes, bool autoWrite = true, bool showStr = true, bool showCursor = true, bool enableKeypad = true);

            //
            // RENDERING THE WINDOW
            //

            /// @brief Render Instantly - Render the window instantly
            void rinst();

            /// @brief Render by Line - Render the window line-by-line (and char-by-char if indicated)
            /// @param dir Direction to start from: 0 = Top-Left, 1 = Bottom-Left, 2 = Left-Top, 3 = Right-Top
            /// @param full Whether to render the full line at once or not
            /// @param rev Whether to reverse the char-by-char direction of rendering (has no effect if full is true)
            /// @param millis Milliseconds to wait in between each line/character rendering
            void rline(unsigned char dir = 0, bool full = true, bool rev = false, unsigned long millis = 20);
            /// @brief Render by Line (from Top) - Render the window line-by-line (and char-by-char if indicated) from the top of the window
            /// @param full Whether to render the full line at once or not
            /// @param rev Whether to reverse the char-by-char direction of rendering (has no effect if full is true)
            /// @param millis Milliseconds to wait in between each line/character rendering
            void rlinetop(bool full = true, bool rev = false, unsigned long millis = 20);
            /// @brief Render by Line (from Bottom) - Render the window line-by-line (and char-by-char if indicated) from the bottom of the window
            /// @param full Whether to render the full line at once or not
            /// @param rev Whether to reverse the char-by-char direction of rendering (has no effect if full is true)
            /// @param millis Milliseconds to wait in between each line/character rendering
            void rlinebot(bool full = true, bool rev = false, unsigned long millis = 20);
            /// @brief Render by Line (from Left) - Render the window line-by-line (and char-by-char if indicated) from the left of the window
            /// @param full Whether to render the full line at once or not
            /// @param rev Whether to reverse the char-by-char direction of rendering (has no effect if full is true)
            /// @param millis Milliseconds to wait in between each line/character rendering
            void rlineleft(bool full = true, bool rev = false, unsigned long millis = 20);
            /// @brief Render by Line (from Right) - Render the window line-by-line (and char-by-char if indicated) from the right of the window
            /// @param full Whether to render the full line at once or not
            /// @param rev Whether to reverse the char-by-char direction of rendering (has no effect if full is true)
            /// @param millis Milliseconds to wait in between each line/character rendering
            void rlineright(bool full = true, bool rev = false, unsigned long millis = 20);

            /// @brief Render in a Radial Motion - Render the window by "sweeping" a line in a radial motion and rendering the window as the line passes over each cell
            /// @param divisions Amount of divisions to split the window into and render individually
            /// @param angle The starting angle of the main "sweeper" (expressed in degrees, 0 = midpoint of the right edge of the window)
            /// @param ccw Whether to "sweep" in a counter-clockwise direction or not
            /// @param millis Milliseconds to wait in between each line/character rendering
            /// @param resolution Number that basically dictates how many steps are required to render each division (smaller number = slower, more accurate "sweeps")
            void rrad(unsigned char divisions = 2, double angle = 90, bool ccw = true, unsigned long millis = 5, double resolution = 0.005);

            //
            // LINE DRAWING
            //

            /// @brief Draw Line - Draw a line with unicode box drawing characters
            /// @param y Starting y-position (row) for the line
            /// @param x Starting x-position (col) for the line
            /// @param length Length of the line
            /// @param vertical Whether the line is vertical or not
            /// @param rev Whether the line is reversed or not (normal is up to down or left to right and reverse switches the direction)
            /// @param style A pair consisting of a number corresponding to the main style (0 = Light Hard, 1 = Light Soft, 2 = Heavy Both, 3 = Heavy Horizontal, 4 = Heavy Vertical, 5 = Doubled Both, 6 = Doubled Horizontal, 7 = Doubled Vertical) and a number corresponding to the dash style (0 = None, 1 = Double, 2 = Triple, 3 = Quardruple, 4 = Single 1, 5 = Single 2)
            /// @param color Color pair to use when drawing the line
            /// @param mergeable Whether the line will be able to be merged with future lines
            /// @param canMerge Whether the line will be able to merge with pre-existing box-drawing characters
            void dline(unsigned short y, unsigned short x, unsigned short length, bool vertical = false, bool rev = false, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge);
            /// @brief Draw Vertical Line - Draw a vertical line (automatically merges with pre-existing lines)
            /// @param y Starting y-position (row) for the line
            /// @param x Starting x-position (col) for the line
            /// @param length Length of the line
            /// @param rev Whether the line is reversed or not (normal is up to down or left to right and reverse switches the direction)
            /// @param style A pair consisting of a number corresponding to the main style (0 = Light Hard, 1 = Light Soft, 2 = Heavy Both, 3 = Heavy Horizontal, 4 = Heavy Vertical, 5 = Doubled Both, 6 = Doubled Horizontal, 7 = Doubled Vertical) and a number corresponding to the dash style (0 = None, 1 = Double, 2 = Triple, 3 = Quardruple, 4 = Single 1, 5 = Single 2)
            /// @param color Color pair to use when drawing the line
            /// @param mergeable Whether the line will be able to be merged with future lines
            /// @param canMerge Whether the line will be able to merge with pre-existing box-drawing characters
            void dvline(unsigned short y, unsigned short x, unsigned short length, bool rev = false, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge);
            /// @brief Draw Horizontal Line - Draw a horizontal line (automatically merges with pre-existing lines)
            /// @param y Starting y-position (row) for the line
            /// @param x Starting x-position (col) for the line
            /// @param length Length of the line
            /// @param rev Whether the line is reversed or not (normal is up to down or left to right and reverse switches the direction)
            /// @param style A pair consisting of a number corresponding to the main style (0 = Light Hard, 1 = Light Soft, 2 = Heavy Both, 3 = Heavy Horizontal, 4 = Heavy Vertical, 5 = Doubled Both, 6 = Doubled Horizontal, 7 = Doubled Vertical) and a number corresponding to the dash style (0 = None, 1 = Double, 2 = Triple, 3 = Quardruple, 4 = Single 1, 5 = Single 2)
            /// @param color Color pair to use when drawing the line
            /// @param mergeable Whether the line will be able to be merged with future lines
            /// @param canMerge Whether the line will be able to merge with pre-existing box-drawing characters
            void dhline(unsigned short y, unsigned short x, unsigned short length, bool rev = false, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge);

            /// @brief Draw Box - Draw a box - top left
            /// @param y y-position (row) of the top-left corner of the box
            /// @param x x-position (col) of the top-left corner of the box
            /// @param dimy Height (rows) of the box
            /// @param dimx Length (cols) of the box
            /// @param style A pair consisting of a number corresponding to the main style (0 = Light Hard, 1 = Light Soft, 2 = Heavy Both, 3 = Heavy Horizontal, 4 = Heavy Vertical, 5 = Doubled Both, 6 = Doubled Horizontal, 7 = Doubled Vertical) and a number corresponding to the dash style (0 = None, 1 = Double, 2 = Triple, 3 = Quardruple, 4 = Single 1, 5 = Single 2)
            /// @param color Color pair to use when drawing the line
            /// @param mergeable Whether the line will be able to be merged with future lines
            /// @param canMerge Whether the line will be able to merge with pre-existing box-drawing characters
            void dbox(unsigned short y, unsigned short x, unsigned short dimy, unsigned short dimx, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge);
            /// @brief Draw Box - Draw a box - border
            /// @param style A pair consisting of a number corresponding to the main style (0 = Light Hard, 1 = Light Soft, 2 = Heavy Both, 3 = Heavy Horizontal, 4 = Heavy Vertical, 5 = Doubled Both, 6 = Doubled Horizontal, 7 = Doubled Vertical) and a number corresponding to the dash style (0 = None, 1 = Double, 2 = Triple, 3 = Quardruple, 4 = Single 1, 5 = Single 2)
            /// @param color Color pair to use when drawing the line
            /// @param mergeable Whether the line will be able to be merged with future lines
            /// @param canMerge Whether the line will be able to merge with pre-existing box-drawing characters
            void dbox(std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge);
            /// @brief Draw Centered Box - Draw a box - center
            /// @param y y-position (row) of the center of the box
            /// @param x x-position (col) of the center of the box
            /// @param dimy Height (rows) of the box
            /// @param dimx Length (cols) of the box
            /// @param style A pair consisting of a number corresponding to the main style (0 = Light Hard, 1 = Light Soft, 2 = Heavy Both, 3 = Heavy Horizontal, 4 = Heavy Vertical, 5 = Doubled Both, 6 = Doubled Horizontal, 7 = Doubled Vertical) and a number corresponding to the dash style (0 = None, 1 = Double, 2 = Triple, 3 = Quardruple, 4 = Single 1, 5 = Single 2)
            /// @param color Color pair to use when drawing the line
            /// @param mergeable Whether the line will be able to be merged with future lines
            /// @param canMerge Whether the line will be able to merge with pre-existing box-drawing characters
            void dcbox(unsigned short y, unsigned short x, unsigned short dimy, unsigned short dimx, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge);

            /// @brief Draw Grid - Draw a grid - top left
            /// @param y y-position of the top-left corner of the grid
            /// @param x x-position of the top-left corner of the grid
            /// @param rows Amount of rows in the grid
            /// @param cols Amount of columns in the grid
            /// @param celly Amount of characters between the horizontal lines that make the cell borders
            /// @param cellx Amount of characters between the vertical lines that make the cell borders
            /// @param style A pair consisting of a number corresponding to the main style (0 = Light Hard, 1 = Light Soft, 2 = Heavy Both, 3 = Heavy Horizontal, 4 = Heavy Vertical, 5 = Doubled Both, 6 = Doubled Horizontal, 7 = Doubled Vertical) and a number corresponding to the dash style (0 = None, 1 = Double, 2 = Triple, 3 = Quardruple, 4 = Single 1, 5 = Single 2)
            /// @param color Color pair to use when drawing the line
            /// @param mergeable Whether the line will be able to be merged with future lines
            /// @param canMerge Whether the line will be able to merge with pre-existing box-drawing characters
            void dgrid(unsigned short y, unsigned short x, unsigned short rows, unsigned short cols, unsigned short celly, unsigned short cellx, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = true);
            /// @brief Draw Grid - Draw a square grid - top left
            /// @param y y-position of the top-left corner of the grid
            /// @param x x-position of the top-left corner of the grid
            /// @param cells Amount of cells in the x and y direction
            /// @param cellDim Amount of characters between the horizontal lines that make the cell borders (this number is doubled for the x-dim of each cell)
            /// @param style A pair consisting of a number corresponding to the main style (0 = Light Hard, 1 = Light Soft, 2 = Heavy Both, 3 = Heavy Horizontal, 4 = Heavy Vertical, 5 = Doubled Both, 6 = Doubled Horizontal, 7 = Doubled Vertical) and a number corresponding to the dash style (0 = None, 1 = Double, 2 = Triple, 3 = Quardruple, 4 = Single 1, 5 = Single 2)
            /// @param color Color pair to use when drawing the line
            /// @param mergeable Whether the line will be able to be merged with future lines
            /// @param canMerge Whether the line will be able to merge with pre-existing box-drawing characters
            void dgrid(unsigned short y, unsigned short x, unsigned short cells, unsigned short cellDim, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge);
            /// @brief Draw Centered Grid - Draw a grid - center
            /// @param y y-position of the center corner of the grid
            /// @param x x-position of the center corner of the grid
            /// @param rows Amount of rows in the grid
            /// @param cols Amount of columns in the grid
            /// @param celly Amount of characters between the horizontal lines that make the cell borders
            /// @param cellx Amount of characters between the vertical lines that make the cell borders
            /// @param style A pair consisting of a number corresponding to the main style (0 = Light Hard, 1 = Light Soft, 2 = Heavy Both, 3 = Heavy Horizontal, 4 = Heavy Vertical, 5 = Doubled Both, 6 = Doubled Horizontal, 7 = Doubled Vertical) and a number corresponding to the dash style (0 = None, 1 = Double, 2 = Triple, 3 = Quardruple, 4 = Single 1, 5 = Single 2)
            /// @param color Color pair to use when drawing the line
            /// @param mergeable Whether the line will be able to be merged with future lines
            /// @param canMerge Whether the line will be able to merge with pre-existing box-drawing characters
            void dcgrid(unsigned short y, unsigned short x, unsigned short rows, unsigned short cols, unsigned short celly, unsigned short cellx, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge);
            /// @brief Draw Centered Grid - Draw a square grid - center
            /// @param y y-position of the center of the grid
            /// @param x x-position of the center of the grid
            /// @param cells Amount of cells in the x and y direction
            /// @param cellDim Amount of characters between the horizontal lines that make the cell borders (this number is doubled for the x-dim of each cell)
            /// @param style A pair consisting of a number corresponding to the main style (0 = Light Hard, 1 = Light Soft, 2 = Heavy Both, 3 = Heavy Horizontal, 4 = Heavy Vertical, 5 = Doubled Both, 6 = Doubled Horizontal, 7 = Doubled Vertical) and a number corresponding to the dash style (0 = None, 1 = Double, 2 = Triple, 3 = Quardruple, 4 = Single 1, 5 = Single 2)
            /// @param color Color pair to use when drawing the line
            /// @param mergeable Whether the line will be able to be merged with future lines
            /// @param canMerge Whether the line will be able to merge with pre-existing box-drawing characters
            void dcgrid(unsigned short y, unsigned short x, unsigned short cells, unsigned short cellDim, std::pair<unsigned char, unsigned char> style = Defaults.Style, unsigned char color = Defaults.Color, bool mergeable = Defaults.Mergeable, bool canMerge = Defaults.CanMerge);

            /// @brief Get Grid Dimensions - Figure out the x and y dimensions of a grid with the given amount of rows/cols and cell sizes
            /// @param rows Amount of rows in the grid
            /// @param cols Amount of columns in the grid
            /// @param celly Amount of characters between the horizontal lines that make the cell borders
            /// @param cellx Amount of characters between the vertical lines that make the cell borders
            /// @returns A pair consisting of a y-dimension (rows) and x-dimension (cols)
            std::pair<unsigned short, unsigned short> gGridDims(unsigned short rows, unsigned short cols, unsigned short celly, unsigned short cellx);
    } mwin;
}
