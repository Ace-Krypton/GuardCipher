/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/passwords.hpp"

auto passwords::add(categories &category) -> void {
    std::string password;

    std::function<void(std::string&)> add_recursive = [&](std::string &password) -> void {
        fmt::print("Enter the Password: ");
        std::cin >> password;
        if (!is_secure(password)) {
            fmt::print("[-] Password is not Secure, Try Again\n");
            add_recursive(password);
        }
    };
    add_recursive(password);

    fmt::print("[+] Password Created Successfully");

    if (!category.print()) return;
    fmt::print("Choose Category to Add Password: ");

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
        std::getline(std::cin, confirmation);
        if (confirmation.size() == 1 && std::toupper(confirmation[0]) == 'Y') {
            selected_category->passwords.emplace_back(password);
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
