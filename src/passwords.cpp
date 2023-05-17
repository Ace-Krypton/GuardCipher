/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/passwords.hpp"

auto passwords::add(categories &category) -> void {
    std::string password_input;

    std::function<void(std::string&)> add_recursive = [&](std::string &password) -> void {
        fmt::print("\nEnter the Password: ");
        std::cin >> password;
        if (!is_secure(password)) {
            fmt::print("[-] Password is not Secure, Try Again\n");
            add_recursive(password);
        }
    };
    add_recursive(password_input);

    fmt::print("\n[+] Password Created Successfully\n\n");
    fmt::print("---------------------------------------\n");

    fmt::print("Do You Want to Add \"{}\", to Categories? (Y/N):", password_input);
    std::string confirmation_adding;
    std::cin >> confirmation_adding;

    if (!category.print() || (confirmation_adding.size() == 1
                        && std::toupper(confirmation_adding[0]) == 'Y')) {
        fmt::print("[+] Adding to Password List");
        struct password new_password;
        new_password.name = password_input;
        new_password.ID = _current_ID++;
        _pass_without_categories[new_password.ID] = new_password;
        fmt::print("\n[+] Password Added to List Successfully\n");
        return;
    }

    fmt::print("\nChoose Category to Add Password: ");

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

    std::optional<categories::category> selected_category = category.get(identifier);
    if (selected_category.has_value()) {
        fmt::print("Are You Sure You Want to Add Password to This Category '{}'? (Y/N): ",
                   selected_category->name);
        std::string confirmation;
        std::cin >> confirmation_adding;
        if (confirmation.size() == 1 && std::toupper(confirmation[0]) == 'Y') {
            auto category_it = category.categories_map.find(selected_category->ID);
            category_it->second.passwords.emplace_back(password_input);
            fmt::print("[+] Password Added Successfully\n");
        } else fmt::print("[-] Canceled\n");

    } else fmt::print("[-] Category Not Found\n");
}

auto passwords::is_secure(const std::string &password) -> bool {
    if (password.length() < 8) return false;

    bool has_numeric = false;
    bool has_upper_case = false;
    bool has_lower_case = false;

    for (char c : password) {
        has_numeric    |= isdigit(c);
        has_upper_case |= isupper(c);
        has_lower_case |= islower(c);
    }

    if (!has_upper_case || !has_lower_case || !has_numeric) return false;

    const std::regex SPECIAL_CHARS(R"([!@#$%^&*()\[\]{}|;:'",.<>/?])");
    if (!std::regex_search(password, SPECIAL_CHARS)) return false;

    const std::regex COMMON_PATTERNS("(\\w)\\1{2,}");
    if (std::regex_search(password, COMMON_PATTERNS)) return false;

    return true;
}
