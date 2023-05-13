/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#pragma once

#include <string>
#include <cstdint>
#include <variant>
#include <optional>
#include <iostream>
#include <unordered_map>

class AddCategory {
public:
    struct Category {
        uint16_t id { };
        std::string category_name;
    };

    auto add_category(const std::string &category_name) -> void;
    auto get_category_by_ID(uint16_t category_ID) const -> std::optional<Category>;
    auto get_category_by_name(const std::string &category_name) const -> std::optional<Category>;
    auto getCategory(const std::variant<int, std::string> &identifier) const -> std::optional<Category>;

private:
    uint16_t current_ID = 1;
    std::unordered_map<int, Category> categories;
};
