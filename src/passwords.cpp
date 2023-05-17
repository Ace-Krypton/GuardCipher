/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/passwords.hpp"

auto passwords::read_input(const std::string& prompt, const std::string& error_message) -> int {
    while (true) {
        std::cout << prompt;
        int value;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << error_message << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

auto passwords::add(categories& category) -> void {
    std::string password_input;
    fmt::print("Choose password generation method:\n");
    fmt::print("1. Manual\n");
    fmt::print("2. Automatic\n");
    int choice = read_input("Enter your choice (1 or 2): ",
                            "Invalid input. Please enter a number.");

    if (choice == 2) {
        int password_length = read_input("Password Length: ",
                                         "Invalid input. Please enter a number.");
        bool has_upper_case = read_input("Include Uppercase Letters (1 for yes, 0 for no): ",
                                         "Invalid input. Please enter either 0 or 1.") != 0;
        bool has_lower_case = read_input("Include Lowercase Letters (1 for yes, 0 for no): ",
                                         "Invalid input. Please enter either 0 or 1.") != 0;
        bool has_special_chars = read_input("Include Special Characters (1 for yes, 0 for no): ",
                                            "Invalid input. Please enter either 0 or 1.") != 0;
        password_input = generator(password_length, has_upper_case, has_lower_case,
                                   has_special_chars);
    } else if (choice == 1) {
        std::function<void(std::string&)> add_recursive = [&](std::string& password) -> void {
            fmt::print("\nEnter the Password: ");
            std::cin >> password;
            if (!is_secure(password)) {
                fmt::print("[-] Password is not Secure, Try Again\n");
                add_recursive(password);
            }
        };
        add_recursive(password_input);
    } else {
        fmt::print("Invalid choice. Exiting password creation.\n");
        return;
    }

    fmt::print("\n[+] Password Created Successfully\n\n");
    fmt::print("---------------------------------------\n");

    fmt::print("Do You Want to Add \"{}\", to Categories? (Y/N):", password_input);
    std::string confirmation_adding;
    std::cin >> confirmation_adding;

    if (!category.print() || (confirmation_adding.size() == 1 && std::toupper(confirmation_adding[0]) == 'Y')) {
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
        fmt::print("Are You Sure You Want to Add Password to This Category '{}'? (Y/N): ", selected_category->name);
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

auto passwords::generator(int password_length, bool has_upper_case,
                          bool has_lower_case, bool has_special_chars) -> std::string {
    std::string characters;

    if (has_lower_case) characters += "abcdefghijklmnopqrstuvwxyz";
    if (has_upper_case) characters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (has_special_chars) characters += "!@#$%^&*()_+";

    if (characters.empty()) {
        fmt::print("[-] No Character Type Selected For Password Generation\n");
        return { };
    }

    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_int_distribution<std::size_t> char_distribution(0, characters.length() - 1);
    std::string password;
    password.reserve(password_length);

    std::generate_n(std::back_inserter(password),
                    password_length, [&]() -> char {
        return characters[char_distribution(generator)];
    });

    return password;
}
