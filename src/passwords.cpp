/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/passwords.hpp"

auto passwords::add() -> void {
    std::function<void()> add_recursive = [&add_recursive]() -> void {
        fmt::print("Enter the Password: ");
        std::string password;
        std::cin >> password;
        if (!is_secure(password)) {
            fmt::print("[-] Password is not Secure, Try Again\n");
            add_recursive();
        }
    };

    add_recursive();
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
