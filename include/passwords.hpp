/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#pragma once

#include <regex>

#include "../include/categories.hpp"

class passwords {
public:
    static auto add() -> void;
    static auto is_secure(const std::string &password) -> bool;
};
