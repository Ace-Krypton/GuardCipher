/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/categories.hpp"

auto categories::add_category() -> void {
    category new_category;

    fmt::print("Enter the category Name: ");
    std::string category_name;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, category_name);

    new_category.category_name = category_name;
    new_category.id = _current_ID++;
    _categories[new_category.id] = new_category;
    fmt::print("\n[+] category Added Successfully\n");
    print_category();
}

auto categories::get_category_by_ID(std::size_t category_ID) const -> std::optional<category> {
    auto it = _categories.find(category_ID);
    if (it != _categories.end()) return it->second;
    return std::nullopt;
}

auto categories::get_category_by_name(const std::string &category_name) const -> std::optional<category> {
    for (const auto &element : _categories) {
        if (element.second.category_name == category_name) {
            return element.second;
        }
    }
    return std::nullopt;
}

auto categories::get_category(const std::variant<std::size_t,
                               std::string> &identifier) const -> std::optional<category> {
    if (std::holds_alternative<std::size_t>(identifier)) {
        std::size_t category_ID = std::get<std::size_t>(identifier);
        return get_category_by_ID(category_ID);
    } else if (std::holds_alternative<std::string>(identifier)) {
        std::string category_name = std::get<std::string>(identifier);
        return get_category_by_name(category_name);
    }
    return std::nullopt;
}

auto categories::print_category() -> bool {
    bool has_categories = !_categories.empty();
    if (has_categories) {
        for (const auto &category : _categories) {
            fmt::print("[+] ID: {} Name: {}\n",
                       category.second.id, category.second.category_name);
        }
    } else fmt::print("[-] No category Found\n");

    return has_categories;
}

auto categories::delete_category() -> void {
    if (!print_category()) return;
    fmt::print("Choose category to Delete: ");

    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    std::variant<std::size_t, std::string> identifier;
    try {
        std::size_t id = std::stoull(input);
        identifier = id;
    } catch (...) {
        identifier = input;
    }

    std::optional<category> category = get_category(identifier);
    if (category.has_value()) {
        fmt::print("Are You Sure You Want to Delete the category '{}'? (Y/N): ",
                   category->category_name);
        std::string confirmation;
        std::getline(std::cin, confirmation);
        if (confirmation.size() == 1 && std::toupper(confirmation[0]) == 'Y') {
            _categories.erase(category->id);
            fmt::print("[+] category Deleted Successfully\n");
        } else fmt::print("[-] Deletion Canceled\n");

    } else fmt::print("[-] category Not Found\n");
}
