/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/AddCategory.hpp"

auto AddCategory::add_category(const std::string &category_name) -> void {
    Category new_category;
    new_category.category_name = category_name;
    new_category.id = _current_ID++;
    _categories[new_category.id] = new_category;
    fmt::print("[+] Category Name Added Successfully\n");
}

auto AddCategory::get_category_by_ID(std::size_t category_ID) const -> std::optional<Category> {
    auto it = _categories.find(category_ID);
    if (it != _categories.end()) return it->second;
    return std::nullopt;
}

auto AddCategory::get_category_by_name(const std::string &category_name) const -> std::optional<Category> {
    for (const auto &element : _categories) {
        if (element.second.category_name == category_name) {
            return element.second;
        }
    }
    return std::nullopt;
}

[[maybe_unused]] auto AddCategory::get_category(const std::variant<std::size_t,
                               std::string> &identifier) const -> std::optional<Category> {
    if (std::holds_alternative<std::size_t>(identifier)) {
        std::size_t category_ID = std::get<std::size_t>(identifier);
        return get_category_by_ID(category_ID);
    } else if (std::holds_alternative<std::string>(identifier)) {
        std::string category_name = std::get<std::string>(identifier);
        return get_category_by_name(category_name);
    }
    return std::nullopt;
}
