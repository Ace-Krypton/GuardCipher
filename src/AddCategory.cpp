/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/AddCategory.hpp"

auto AddCategory::add_category(const std::string &category_name) -> void {
    Category new_category;
    new_category.category_name = category_name;
    new_category.id = current_ID++;

    categories[new_category.id] = new_category;
}

auto AddCategory::get_category_by_ID(uint16_t category_ID) const -> std::optional<Category> {
    auto it = categories.find(category_ID);
    if (it != categories.end()) return it->second;
    return std::nullopt;
}

auto AddCategory::get_category_by_name(const std::string &category_name) const -> std::optional<Category> {
    for (const auto &element : categories) {
        if (element.second.category_name == category_name) {
            return element.second;
        }
    }
    return std::nullopt;
}

auto AddCategory::getCategory(const std::variant<int, std::string> &identifier) const -> std::optional<Category> {
    return std::optional<Category>();
}

