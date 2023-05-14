/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/Categories.hpp"

auto Categories::add_category(const std::string &category_name) -> void {
    Category new_category;
    new_category.category_name = category_name;
    new_category.id = _current_ID++;
    _categories[new_category.id] = new_category;
    fmt::print("[+] Category Added Successfully\n");
}

auto Categories::get_category_by_ID(std::size_t category_ID) const -> std::optional<Category> {
    auto it = _categories.find(category_ID);
    if (it != _categories.end()) return it->second;
    return std::nullopt;
}

auto Categories::get_category_by_name(const std::string &category_name) const -> std::optional<Category> {
    for (const auto &element : _categories) {
        if (element.second.category_name == category_name) {
            return element.second;
        }
    }
    return std::nullopt;
}

auto Categories::get_category(const std::variant<std::size_t,
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

auto Categories::print_category() -> void {
    if (!_categories.empty()) {
        for (const auto &category : _categories) {
            fmt::print("[+] ID: {} Name: {}\n",
                       category.second.id ,category.second.category_name);
        }
    } else fmt::print("\n[-] No Category Found\n");
}

auto Categories::delete_category() -> void {
    print_category();
    fmt::print("Choose Category to Delete: ");

    std::string input;
    std::cin >> input;

    std::variant<std::size_t, std::string> identifier;
    try {
        std::size_t id = std::stoull(input);
        identifier = id;
    } catch (...) {
        identifier = input;
    }

    std::optional<Categories::Category> category = get_category(identifier);
    if (category.has_value()) {
        _categories.erase(category->id);
        fmt::print("Category deleted successfully.\n");
    } else fmt::print("Category not found.\n");
}
