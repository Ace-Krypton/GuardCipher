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

    static auto generator(int password_length, bool has_upper_case,
                   bool has_lower_case, bool has_special_chars) -> std::string;
    static auto read_input(const std::string& prompt, const std::string& error_message) -> int;
    auto add(categories &category) -> void;
    static auto is_secure(const std::string &password) -> bool;

private:
    std::size_t _current_ID = 1;
    std::map<std::size_t, password> _pass_without_categories;
};
