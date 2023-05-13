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
#include <fmt/core.h>
#include <unordered_map>

class AddCategory {
public:
    struct Category {
        std::size_t id { };
        std::string category_name;
    };

    auto add_category(const std::string &category_name) -> void;
    auto get_category_by_ID(std::size_t category_ID) const -> std::optional<Category>;
    auto get_category_by_name(const std::string &category_name) const -> std::optional<Category>;

    [[maybe_unused]] auto get_category(const std::variant<std::size_t,
                                       std::string> &identifier) const -> std::optional<Category>;

private:
    std::size_t _current_ID = 1;
    std::unordered_map<std::size_t, Category> _categories;
};
