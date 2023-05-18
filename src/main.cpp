/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/menu.hpp"

auto main() -> int {
    passwords password;
    categories category;

    std::vector<menu::item> menu {
            {1, "Add Category"},
            {2, "Remove Category"},
            {3, "Print Category"},
            {4, "Search for Passwords"},
            {5, "Sort Passwords"},
            {6, "Add Password"},
            {7, "Edit Password"},
            {8, "Remove Password"},
            {9, "Write Changes To File"},
            {0, "Exit"},
    };

    menu::process(password, category, menu);

    return 0;
}
