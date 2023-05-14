/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/Menu.hpp"

auto main() -> int {
    std::vector<Menu::MenuItem> menu {
            {1, "Add a category"},
            {2, "Remove a category"},
            {3, "Print a category"},
            {4, "Search for passwords"},
            {5, "Sort passwords"},
            {6, "Add a password"},
            {7, "Edit a password"},
            {8, "Remove a password"},
            {9, "Write changes to a file"},
            {0, "Exit"},
    };

    Menu::process_menu(menu);

    return 0;
}
