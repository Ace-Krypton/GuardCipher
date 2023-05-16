/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#pragma once

#include <atomic>
#include <vector>
#include <functional>

#include "categories.hpp"

class menu {
public:
    struct menu_item {
        std::size_t id;
        std::string label;
        std::vector<menu_item> sub_menu;
    };

    static auto display_menu(const std::vector<menu_item> &menu) -> void;
    static auto process_menu(categories &category, const std::vector<menu_item> &menu) -> void;
    static auto handle_menu_option(categories &category, std::size_t option_ID, std::atomic<bool> &flag) -> void;
    static auto find_menu_item(const std::vector<menu_item> &menu, std::size_t id) -> std::optional<menu::menu_item>;
};
