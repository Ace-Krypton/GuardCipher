/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#pragma once

#include <vector>
#include <functional>

#include "AddCategory.hpp"

class Menu {
public:
    struct MenuItem {
        std::size_t id;
        std::string label;
        std::vector<MenuItem> sub_menu;
    };

    static auto process_menu(const std::vector<MenuItem> &menu) -> void;
    static auto display_menu(const std::vector<MenuItem> &menu) -> void;
    static auto find_menu_item(const std::vector<MenuItem> &menu,
                               std::size_t id) -> std::optional<Menu::MenuItem*>;
};
