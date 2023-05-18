/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/categories.hpp"

auto categories::add() -> void {
    struct category new_category;

    fmt::print("Enter the Category Name: ");
    std::string category_name;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, category_name);

    new_category.name = category_name;
    new_category.ID = _current_ID++;
    categories_map[new_category.ID] = new_category;
    fmt::print("\n[+] Category Added Successfully\n");
}

auto categories::get_ID(std::size_t category_ID) const -> std::optional<category> {
    auto it = categories_map.find(category_ID);
    if (it != categories_map.end()) return it->second;
    return std::nullopt;
}

auto categories::get_name(const std::string &category_name) const -> std::optional<category> {
    for (const auto &element : categories_map) {
        if (element.second.name == category_name) {
            return element.second;
        }
    }
    return std::nullopt;
}

auto categories::get(const std::variant<std::size_t,
                     std::string> &identifier) const -> std::optional<category> {
    if (std::holds_alternative<std::size_t>(identifier)) {
        std::size_t category_ID = std::get<std::size_t>(identifier);
        return get_ID(category_ID);
    } else if (std::holds_alternative<std::string>(identifier)) {
        std::string category_name = std::get<std::string>(identifier);
        return get_name(category_name);
    }
    return std::nullopt;
}

auto categories::is_printable() -> bool {
    std::size_t password_id = 1;
    bool has_categories = !categories_map.empty();
    if (has_categories) {
        fmt::print("\n----------- Categories -----------\n");
        for (const auto &category : categories_map) {
            fmt::print("\n[+] ID: {} Name: {}\n Passwords:\n",
                       category.second.ID, category.second.name);
            for (const std::string &password : category.second.passwords) {
                fmt::print("ID: {} {}\n", password_id, password);
                ++password_id;
            }
        }
    } else fmt::print("\n[-] No Category Found\n");

    return has_categories;
}

auto categories::remove() -> void {
    if (!is_printable()) return;
    fmt::print("Choose Category to Delete: ");

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

    std::optional<category> category_selected = get(identifier);
    if (category_selected.has_value()) {
        fmt::print("Are You Sure You Want to Delete the Category '{}'? (Y/N): ",
                   category_selected->name);
        std::string confirmation;
        std::cin >> confirmation;
        if (confirmation.size() == 1 && std::toupper(confirmation[0]) == 'Y') {
            categories_map.erase(category_selected->ID);
            fmt::print("\n[+] Category Deleted Successfully\n");
        } else fmt::print("\n[-] Canceled\n");

    } else fmt::print("\n[-] Category Not Found\n");
}
