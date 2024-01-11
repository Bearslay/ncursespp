#pragma once

#include "General.hpp"
#include "Window.hpp"

namespace npp {
    /// @brief Essentially the same as the MEVENT from ncurses, but with friendlier values
    class Mouse {
        private:
            /// @brief Last button/input the mouse had
            char Button = -1;
            /// @brief x-position (col) of the mouse cursor during the last event
            unsigned short X = 0;
            /// @brief y-position (row) of the mouse cursor during the last event
            unsigned short Y = 0;
            /// @brief z-position (???) of the mouse cursor during the last event
            unsigned short Z = 0;

        public:
            /// @brief Get Input - Get the mouse's last recorded input
            /// @returns A number corresponding to the mouse's last recorded input
            const char ginput();
            /// @brief Get X - Get the mouse's last recorded x-position (col)
            /// @returns The mouse's last recorded x-position (col)
            const unsigned short gx();
            /// @brief Get Y - Get the mouse's last recorded y-position (row)
            /// @returns The mouse's last recorded y-position (row)
            const unsigned short gy();
            /// @brief Get Z - Get the mouse's last recorded z-position (???)
            /// @returns The mouse's last recorded z-position (???)
            const unsigned short gz();

            /// @brief Get Mouse - Take an input from gchar() or something and update the mouse
            /// @param input Integer input (most likely from gchar())
            /// @returns True if the Mouse was successfully updated, false if not
            bool gmouse(int input);
    } mmouse;
}
