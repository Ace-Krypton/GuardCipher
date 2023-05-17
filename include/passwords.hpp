/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#pragma once

#include <regex>
#include <random>

#include "../include/categories.hpp"

class passwords {
public:
    struct password {
        std::size_t ID;
        std::string name;
    };

    auto generator(int password_length, bool has_upper_case,
                   bool has_lower_case, bool has_special_chars) -> std::string;
    auto add(categories &category) -> void;
    static auto is_secure(const std::string &password) -> bool;

private:
    std::size_t _current_ID = 1;
    std::map<std::size_t, password> _pass_without_categories;
};
