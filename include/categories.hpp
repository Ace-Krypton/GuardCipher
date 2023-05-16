/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#pragma once

#include <map>
#include <string>
#include <cstdint>
#include <variant>
#include <optional>
#include <iostream>
#include <fmt/core.h>

class categories {
public:
    struct category {
        std::size_t id { };
        std::string category_name;
    };

    auto add_category() -> void;
    auto print_category() -> bool;
    auto delete_category() -> void;
    [[nodiscard]] auto get_category_by_ID(std::size_t category_ID) const -> std::optional<category>;
    [[nodiscard]] auto get_category_by_name(const std::string &category_name) const -> std::optional<category>;
    [[nodiscard]] auto get_category(const std::variant<std::size_t,
                                    std::string> &identifier) const -> std::optional<category>;

private:
    std::size_t _current_ID = 1;
    std::map<std::size_t, category> _categories;
};
