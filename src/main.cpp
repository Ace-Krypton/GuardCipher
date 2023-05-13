/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */


#include "../include/Menu.hpp"

auto main() -> int {
    std::vector<Menu::MenuItem> menu {
            {1, "Main Menu", {
                    {1, "Add Category"},
                    {0, "Exit"}
            }}
    };

    Menu::process_menu(menu);

    return 0;
}
