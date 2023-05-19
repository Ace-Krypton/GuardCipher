/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#pragma once

#include <map>
#include <vector>
#include <cstdint>
#include <variant>
#include <optional>
#include <iostream>
#include <fmt/core.h>

class categories {
public:
    struct category {
        std::size_t ID { };
        std::string name;
        std::vector<std::string> passwords;
    };

    auto add() -> void;
    auto remove() -> void;
    auto is_printable() -> bool;
    [[nodiscard]] auto get_ID(std::size_t category_ID) const -> std::optional<category>;
    [[nodiscard]] auto get_name(const std::string &category_name) const -> std::optional<category>;
    [[nodiscard]] auto get(const std::variant<std::size_t,
                           std::string> &identifier) const -> std::optional<category>;

    std::map<std::size_t, category> categories_map;

private:
    std::size_t _current_ID = 1;
};
