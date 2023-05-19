/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/passwords.hpp"

template <typename T>
auto passwords::read_input(const std::string &prompt, const std::string &error_message,
                           const std::vector<T> &valid_values) -> T {
    while (true) {
        fmt::print("{}", prompt);
        T value;
        if (std::cin >> value) {
            if (std::find(valid_values.begin(),
                          valid_values.end(), value) != valid_values.end()) { return value; }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } fmt::print("\n[-] {}\n", error_message);
    }
}


auto passwords::add(categories &category) -> void {
    std::string password_input;
    fmt::print("Choose password generation method:\n");
    fmt::print("[1] Automatic\n");
    fmt::print("[2] Manual\n");
    int choice = read_input<int>("Enter Your Choice (1 or 2): ",
                            "Invalid Input. Please Enter a Correct Number.", {1, 2});

    if (choice == 1) {
        while (true) {
            std::vector<int> valid_lengths(50);
            std::iota(valid_lengths.begin(), valid_lengths.end(), 1);

            int password_length =
                    read_input<int>("\nPassword Length (Max Length 50): ",
                                    "Invalid Range. Please Enter Correct Length.",
                                    valid_lengths);

            bool has_upper_case =
                    read_input<int>("Include Uppercase Letters (1 for yes, 0 for no): ",
                                    "Invalid Input. Please Enter Either 0 or 1.",
                                    {0, 1});

            bool has_lower_case =
                    read_input<int>("Include Lowercase Letters (1 for yes, 0 for no): ",
                                    "Invalid Input. Please Enter Either 0 or 1.",
                                    {0, 1});

            bool has_special_chars =
                    read_input<int>("Include Special Characters (1 for yes, 0 for no): ",
                                    "Invalid Input. Please Enter Either 0 or 1.",
                                    {0, 1});

            password_input = generator(password_length, has_upper_case, has_lower_case, has_special_chars);
            if (password_input != "error occured") break;
        }
    } else if (choice == 2) {
        std::function<void(std::string&)> add_recursive = [&](std::string &password) -> void {
            fmt::print("\nEnter the Password: ");
            std::cin >> password;
            if (!is_secure(password)) {
                fmt::print("\n[-] Password is not Secure, Try Again\n");
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

    if (!category.is_printable() || (confirmation_adding.size() == 1
                                     && std::toupper(confirmation_adding[0]) == 'N')) {
        fmt::print("\n[+] Adding to Password List");
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
        std::cin >> confirmation;
        if (confirmation.size() == 1 && std::toupper(confirmation[0]) == 'Y') {
            auto category_it = category.categories_map.find(selected_category->ID);
            category_it->second.passwords.emplace_back(password_input);
            fmt::print("\n[+] Password Added Successfully\n");
        } else fmt::print("\n[-] Canceled\n");

    } else fmt::print("\n[-] Category Not Found\n");
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
        fmt::print("\n[-] No Character Type Selected\n");
        return "error occured";
    }

    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_int_distribution<std::size_t>
            char_distribution(0, characters.length() - 1);
    std::string password;
    password.reserve(password_length);

    std::generate_n(std::back_inserter(password),
                    password_length, [&]() -> char {
        return characters[char_distribution(generator)];
    });

    return password;
}

auto passwords::search(const categories &category) -> void {
    std::string search_param;
    fmt::print("Enter the Search Parameter: ");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, search_param);

    bool found = false;
    fmt::print("\nSearch Results:\n");

    for (const auto &password : _pass_without_categories) {
        if (password.second.name.find(search_param) != std::string::npos) {
            fmt::print("[ID: {}] {}\n",
                       password.second.ID, password.second.name);
            found = true;
        }
    }

    for (const auto &element : category.categories_map) {
        for (const std::string &password : element.second.passwords) {
            if (password.find(search_param) != std::string::npos) {
                fmt::print("[Category: '{}', ID: {}] {}\n",
                           element.second.name, element.second.ID, password);
                found = true;
            }
        }
    }

    if (!found) fmt::print("\n[-] No Passwords Found\n");
}

auto passwords::sort(categories &category) -> void {
    int sort_option = read_input<int>("Sort Password From:\n[1] Password List\n"
                                 "[2] Category\nEnter your choice: ",
                                 "Invalid input. Please enter a valid option.",
                                 {1, 2});

    if (sort_option == 1) {
        std::vector<std::pair<std::size_t, password>> temp_passwords(
                std::make_move_iterator(_pass_without_categories.begin()),
                std::make_move_iterator(_pass_without_categories.end()));

        std::sort(temp_passwords.begin(),
                  temp_passwords.end(),
                  [](const auto &a,
                          const auto &b) -> bool {
                      return a.second.name < b.second.name;
                  });

        _pass_without_categories = std::map<std::size_t, password>(
                std::make_move_iterator(temp_passwords.begin()),
                std::make_move_iterator(temp_passwords.end()));
    } else if (sort_option == 2) {
        std::vector<categories::category> sorted_categories_vec;
        sorted_categories_vec.reserve(category.categories_map.size());

        for (auto &&element : category.categories_map) {
            sorted_categories_vec.push_back(std::move(element.second));
        }
        std::sort(sorted_categories_vec.begin(),
                  sorted_categories_vec.end(),
                  [](const categories::category &a,
                          const categories::category &b) -> bool {
                      return a.name < b.name;
                  });

        std::map<std::size_t, categories::category> sorted_categories_map;
        std::size_t current_ID = 1;
        for (auto &&sorted_category : sorted_categories_vec) {
            sorted_categories_map[current_ID] = std::move(sorted_category);
            current_ID++;
        }
        category.categories_map = std::move(sorted_categories_map);

        for (auto &element : category.categories_map) {
            std::sort(element.second.passwords.begin(),
                      element.second.passwords.end());
        }
    }

    fmt::print("\n[+] Passwords sorted successfully\n");
}

auto passwords::remove(categories &category) -> void {
    int delete_option = read_input<int>("Delete Password From:\n[1] Password List\n"
                                   "[2] Category\nEnter your choice: ",
                                   "Invalid input. Please enter a valid option.",
                                   {1, 2});

    if (delete_option == 1) {
        if (!is_printable()) {
            fmt::print("\n[-] No Passwords Found Inside of Password List");
            return;
        }

        auto password_id =
                read_input<std::size_t>("\nEnter the ID of the password to delete: ",
                                        "Invalid ID. Please enter a valid ID.",
                                        get_password_ids());

        auto password_it = _pass_without_categories.find(password_id);

        if (password_it != _pass_without_categories.end()) {
            _pass_without_categories.erase(password_it);
            fmt::print("\n[+] Password deleted successfully\n");
        } else fmt::print("\n[-] Password with ID {} not found\n", password_id);

    } else if (delete_option == 2) {
        if (!category.is_printable()) {
            fmt::print("\n[-] No Categories Found");
            return;
        }

        std::variant<std::size_t, std::string> identifier;

        std::string input;
        fmt::print("Enter the Category: ");
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, input);

        try {
            std::size_t id = std::stoull(input);
            identifier = id;
        } catch (...) {
            identifier = input;
        }

        std::optional<categories::category> selected_category = category.get(identifier);
        if (selected_category.has_value()) {
            auto password_id =
                    read_input<std::size_t>("Enter the ID of the password to delete: ",
                                            "Invalid ID. Please enter a valid ID.",
                                            {1, selected_category->passwords.size()});

            if (password_id >= 1 && password_id <= selected_category->passwords.size()) {
                auto category_it = category.categories_map.find(selected_category->ID);
                category_it->second.passwords.erase(
                        selected_category->passwords.begin() + (static_cast<long>(password_id) - 1));
                fmt::print("\n[+] Password Deleted Successfully\n");

            } else fmt::print("\n[-] Invalid Password ID\n");
        } else fmt::print("\n[-] Category Not Found\n");
    }
}

auto passwords::get_password_ids() const -> std::vector<std::size_t> {
    std::vector<std::size_t> password_ids;
    password_ids.reserve(_pass_without_categories.size());

    for (const auto &password : _pass_without_categories) {
        password_ids.push_back(password.first);
    }

    return password_ids;
}

auto passwords::is_printable() -> bool {
    bool has_passwords = !_pass_without_categories.empty();

    if (has_passwords) {
        for (auto const &pass : _pass_without_categories) {
            fmt::print("\n[+] ID: {} Name: {}",
                       pass.second.ID, pass.second.name);
        }
    } else fmt::print("\n[-] No Passwords Found\n");

    return has_passwords;
}

auto passwords::edit(categories &category) -> void {
    int edit_option = read_input<int>("Edit Password From:\n[1] Password List\n"
                                      "[2] Category\nEnter your choice: ",
                                      "Invalid input. Please enter a valid option.",
                                      {1, 2});

    if (edit_option == 1) {
        if (!is_printable()) {
            fmt::print("\n[-] No Passwords Found Inside the Password List");
            return;
        }

        auto password_id =
                read_input<std::size_t>("\nEnter the ID of the password to edit: ",
                                        "Invalid ID. Please enter a valid ID.",
                                        get_password_ids());

        auto password_it = _pass_without_categories.find(password_id);

        if (password_it != _pass_without_categories.end()) {
            auto &password = password_it->second;

            fmt::print("\n[+] Editing Password ID: {}\n", password_id);
            std::string new_password;
            std::cout << "Enter the New Password: ";
            std::cin >> new_password;

            if (is_secure(new_password)) {
                password.name = new_password;
                fmt::print("\n[+] Password Edited Successfully\n");

            } else fmt::print("\n[-] New Password is Not Secure. Please Try Again.\n");

        } else fmt::print("\n[-] Password with ID {} Not Found\n", password_id);

    } else if (edit_option == 2) {
        if (!category.is_printable()) return;

        std::variant<std::size_t, std::string> identifier;

        std::string input;
        fmt::print("Enter the Category: ");
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, input);

        try {
            std::size_t id = std::stoull(input);
            identifier = id;
        } catch (...) {
            identifier = input;
        }

        std::optional<categories::category> selected_category = category.get(identifier);
        if (selected_category.has_value()) {
            auto password_id =
                    read_input<std::size_t>("Enter the ID of the password to edit: ",
                                            "Invalid ID. Please enter a valid ID.",
                                            {1, selected_category->passwords.size()});

            if (password_id >= 1 && password_id <= selected_category->passwords.size()) {
                auto category_it = category.categories_map.find(selected_category->ID);
                std::vector<std::string> &passwords = category_it->second.passwords;
                std::string &password = passwords[password_id - 1];

                fmt::print("\n[+] Editing Password ID: {}\n", password_id);
                std::string new_password;
                std::cout << "Enter the New Password: ";
                std::cin >> new_password;

                if (is_secure(new_password)) {
                    password = new_password;
                    fmt::print("\n[+] Password Edited Successfully\n");

                } else fmt::print("\n[-] New Password is Not Secure. Please Try Again.\n");

            } else fmt::print("\n[-] Invalid Password ID\n");

        } else fmt::print("\n[-] Category Not Found\n");
    }
}
