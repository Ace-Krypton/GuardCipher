/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/menu.hpp"

auto main() -> int {
    categories category;

    std::vector<menu::item> menu {
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

    menu::process(category, menu);

    return 0;
}
