/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#pragma once

#include <atomic>
#include <vector>
#include <functional>

#include "cryptor.hpp"
#include "passwords.hpp"
#include "categories.hpp"

class menu {
public:
    struct item {
        std::size_t id;
        std::string label;
        std::vector<item> sub_menu;
    };

    static auto display(const std::vector<item> &menu) -> void;
    static auto process(passwords &password, categories &category, const std::vector<item> &menu) -> void;
    static auto find_item(const std::vector<item> &menu, std::size_t id) -> std::optional<menu::item>;
    static auto handle_option(passwords &password, categories &category,
                              std::size_t option_ID, std::atomic<bool> &flag) -> void;
};
