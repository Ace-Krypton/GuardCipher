/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/passwords.hpp"

/**
 * @brief Reads input from the user.
 * @tparam T The type of the input value.
 * @param prompt The prompt message displayed to the user.
 * @param error_message The error message displayed when an invalid input is provided.
 * @param valid_values The list of valid values to validate the input against.
 * @return The valid input value provided by the user.
 */
template <typename T>
auto passwords::read_input(const std::string &prompt, const std::string &error_message,
                           const std::vector<T> &valid_values) -> T {
    while (true) {
        fmt::print("{}", prompt);
        T value;

        /// Read input from standard input into 'value'
        if (std::cin >> value) {
            /// Check if 'value' is present in the 'valid_values' vector
            if (std::find(valid_values.begin(),
                          valid_values.end(), value) != valid_values.end()) { return value; }
        } else {
            /// Clear any error flags and ignore remaining input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        /// Display the error message
        fmt::print("\n[-] {}\n", error_message);
    }
}

/**
 * @brief Adds a password to the password manager, either with categories or without.
 * @param category The categories object to add the password to.
 */
auto passwords::add(categories &category) -> void {
    std::string password_input;
    fmt::print("Choose password generation method:\n");
    fmt::print("[1] Automatic\n");
    fmt::print("[2] Manual\n");

    /// Read the user's choice for password generation method (1 or 2)
    int choice = read_input<int>("Enter Your Choice (1 or 2): ",
                            "Invalid Input. Please Enter a Correct Number.", {1, 2});

    if (choice == 1) {
        /// Automatic password generation method
        while (true) {
            /// Generate a vector of valid lengths from 1 to 50
            std::vector<int> valid_lengths(50);
            std::iota(valid_lengths.begin(), valid_lengths.end(), 1);

            /// Read the password length from the user
            int password_length =
                    read_input<int>("\nPassword Length (Max Length 50): ",
                                    "Invalid Range. Please Enter Correct Length.",
                                    valid_lengths);

            /// Read whether to include uppercase letters from the user
            bool has_upper_case =
                    read_input<int>("Include Uppercase Letters (1 for yes, 0 for no): ",
                                    "Invalid Input. Please Enter Either 0 or 1.",
                                    {0, 1});

            /// Read whether to include lowercase letters from the user
            bool has_lower_case =
                    read_input<int>("Include Lowercase Letters (1 for yes, 0 for no): ",
                                    "Invalid Input. Please Enter Either 0 or 1.",
                                    {0, 1});

            /// Read whether to include special characters from the user
            bool has_special_chars =
                    read_input<int>("Include Special Characters (1 for yes, 0 for no): ",
                                    "Invalid Input. Please Enter Either 0 or 1.",
                                    {0, 1});

            /// Generate the password using the provided specifications
            password_input = generator(password_length, has_upper_case,
                                       has_lower_case, has_special_chars);

            /// Check if an error occurred during password generation,
            /// and repeat the process if necessary
            if (password_input != "error occured") break;
        }
    } else if (choice == 2) {
        /// Manual password generation method
        std::function<void(std::string&)> add_recursive =
                [&](std::string &password) -> void {
            fmt::print("\nEnter the Password: ");
            std::cin >> password;
            /// Check if the manually entered password is secure, and prompt again if it's not
            if (!is_secure(password)) {
                fmt::print("\n[-] Password is not Secure, Try Again\n");
                add_recursive(password);
            }
        };
        /// Call the recursive function to add the manually entered password
        add_recursive(password_input);
    } else {
        fmt::print("[-] Invalid Choice. Exiting Password Creation.\n");
        return;
    }

    fmt::print("\n[+] Password Created Successfully\n\n");
    fmt::print("---------------------------------------\n");

    fmt::print("Do You Want to Add \"{}\", to Categories? (Y/N):", password_input);
    std::string confirmation_adding;
    std::cin >> confirmation_adding;

    /// Check if the password should be added to categories
    if (!category.is_printable() || (confirmation_adding.size() == 1
                                     && std::toupper(confirmation_adding[0]) == 'N')) {
        /// Add the password to the list without categories
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

    /// Try to convert the input to a category identifier
    /// (either a size_t or a string)
    std::variant<std::size_t, std::string> identifier;
    try {
        std::size_t id = std::stoull(input);
        identifier = id;
    } catch (...) {
        identifier = input;
    }

    /// Get the selected category based on the identifier
    std::optional<categories::category> selected_category = category.get(identifier);
    if (selected_category.has_value()) {
        /// Confirm adding the password to the selected category
        fmt::print("Are You Sure You Want to Add Password to This Category '{}'? (Y/N): ",
                   category.categories_map.find(selected_category->ID)->second.name);
        std::string confirmation;
        std::cin >> confirmation;
        if (confirmation.size() == 1 && std::toupper(confirmation[0]) == 'Y') {
            /// Add the password to the selected category
            auto category_it = category.categories_map.find(selected_category->ID);
            category_it->second.passwords[category_it->second._pass_id++] = password_input;
            fmt::print("\n[+] Password Added Successfully\n");
        } else fmt::print("\n[-] Canceled\n");

    } else fmt::print("\n[-] Category Not Found\n");
}

/**
 * @brief Checks if a password is secure based on certain criteria.
 * @param password The password to check.
 * @return True if the password is secure, false otherwise.
 */
auto passwords::is_secure(const std::string &password) -> bool {
    /// Check if the password length is less than 8 characters
    if (password.length() < 8) return false;

    /// Initialize variables to track if the password contains numeric,
    /// uppercase, and lowercase characters
    bool has_numeric = false;
    bool has_upper_case = false;
    bool has_lower_case = false;

    /// Iterate over each character in the password
    for (char c : password) {
        has_numeric    |= isdigit(c);
        has_upper_case |= isupper(c);
        has_lower_case |= islower(c);
    }

    /// Check if the password contains at least one uppercase,
    /// one lowercase, and one numeric character
    if (!has_upper_case || !has_lower_case || !has_numeric) return false;

    // Define a regex pattern for special characters
    const std::regex SPECIAL_CHARS(R"([!@#$%^&*()\[\]{}|;:'",.<>/?])");
    /// Check if the password contains at least one special character
    if (!std::regex_search(password, SPECIAL_CHARS)) return false;

    /// Define a regex pattern for common repeating patterns
    const std::regex COMMON_PATTERNS("(\\w)\\1{2,}");
    /// Check if the password contains common repeating patterns
    if (std::regex_search(password, COMMON_PATTERNS)) return false;

    /// If none of the previous conditions matched,
    /// then the password is considered secure
    return true;
}

/**
 * @brief Generates a password with the specified length and character types.
 * @param password_length The length of the password.
 * @param has_upper_case Flag indicating whether the password should contain uppercase letters.
 * @param has_lower_case Flag indicating whether the password should contain lowercase letters.
 * @param has_special_chars Flag indicating whether the password should contain special characters.
 * @return The generated password.
 */
auto passwords::generator(int password_length, bool has_upper_case,
                          bool has_lower_case, bool has_special_chars) -> std::string {
    /// Create an empty string to store
    /// the possible characters for the password
    std::string characters;

    /// Add lowercase letters to the character set if required
    if (has_lower_case) characters += "abcdefghijklmnopqrstuvwxyz";
    /// Add uppercase letters to the character set if required
    if (has_upper_case) characters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    /// Add special characters to the character set if required
    if (has_special_chars) characters += "!@#$%^&*()_+";

    /// Check if no character type was selected
    if (characters.empty()) {
        fmt::print("\n[-] No Character Type Selected\n");
        return "error occured";
    }

    /// Create a random device and a random number generator
    std::random_device rd;
    std::mt19937 generator(rd());

    /// Define a uniform distribution for selecting characters from the character set
    std::uniform_int_distribution<std::size_t>
            char_distribution(0, characters.length() - 1);
    /// Create a string to store the generated password
    std::string password;
    password.reserve(password_length);

    /// Generate the password by randomly selecting characters from the character set
    std::generate_n(std::back_inserter(password),
                    password_length, [&]() -> char {
        return characters[char_distribution(generator)];
    });

    return password;
}

/**
 * @brief Searches for passwords matching the given search parameter within
 * the password list and categories.
 * @param category The category object.
 */
auto passwords::search(const categories &category) -> void {
    /// Initialize a string to store the search parameter
    std::string search_param;
    fmt::print("Enter the Search Parameter: ");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, search_param);

    /// Initialize a boolean variable
    /// to track if any passwords are found
    bool found = false;
    fmt::print("\nSearch Results:\n");

    /// Iterate over passwords without categories
    for (const auto &password : _pass_without_categories) {
        if (password.second.name.find(search_param) != std::string::npos) {
            fmt::print("[ID: {}] {}\n",
                       password.second.ID, password.second.name);
            found = true;
        }
    }

    /// Iterate over categories and their passwords
    for (const auto &element : category.categories_map) {
        for (const auto &password : element.second.passwords) {
            /// Check if the password contains the search parameter
            if (password.second.find(search_param) != std::string::npos) {
                fmt::print("[Category: '{}', ID: {}] {}\n",
                           element.second.name, element.second.ID, password.second);
                found = true;
            }
        }
    }

    if (!found) fmt::print("\n[-] No Passwords Found\n");
}

/**
 * @brief Sorts the passwords either in the password list or within each category.
 * @param category The category object.
 */
auto passwords::sort(categories &category) -> void {
    int sort_option = read_input<int>("Sort Password From:\n[1] Password List\n"
                                      "[2] Category\nEnter your choice: ",
                                      "Invalid input. Please enter a valid option.",
                                      {1, 2});

    /// Sort passwords in the password list (_pass_without_categories) by name
    if (sort_option == 1) {
        /// Create temp vector for being able to use std::sort
        std::vector<std::pair<std::size_t, password>> temp_passwords(
                std::make_move_iterator(_pass_without_categories.begin()),
                std::make_move_iterator(_pass_without_categories.end()));

        /// Sorting the passwords by name
        std::sort(temp_passwords.begin(),
                  temp_passwords.end(),
                  [](const auto &a,
                          const auto &b) -> bool {
                      return a.second.name < b.second.name;
                  });

        /// Update the password list with the sorted passwords
        _pass_without_categories = std::map<std::size_t, password>(
                std::make_move_iterator(temp_passwords.begin()),
                std::make_move_iterator(temp_passwords.end()));

    } else if (sort_option == 2) {
        /// Sort categories and their passwords
        std::vector<categories::category> sorted_categories_vec;
        sorted_categories_vec.reserve(category.categories_map.size());

        /// Move categories from the original map to a vector for sorting
        /// Used move semantics to be efficient and not to create copies
        for (auto &&element : category.categories_map) {
            sorted_categories_vec.push_back(std::move(element.second));
        }

        /// Sort the categories by name
        std::sort(sorted_categories_vec.begin(),
                  sorted_categories_vec.end(),
                  [](const categories::category &a,
                          const categories::category &b) -> bool {
                      return a.name < b.name;
                  });

        /// Create a new map for sorted categories
        std::map<std::size_t, categories::category> sorted_categories_map;
        std::size_t current_ID = 1;

        /// Move the sorted categories from the vector to the new map
        for (auto &&sorted_category : sorted_categories_vec) {
            sorted_categories_map[current_ID] = std::move(sorted_category);
            current_ID++;
        }

        /// Update the categories map with the sorted categories
        category.categories_map = std::move(sorted_categories_map);

        /// Sort passwords within each category
        for (auto &element : category.categories_map) {
            /// Create temp vector with pairs in order to use std::sort
            std::vector<std::pair<std::size_t, std::string>> temp_passwords(
                    element.second.passwords.begin(),
                    element.second.passwords.end());

            /// Sort the passwords by name
            std::sort(temp_passwords.begin(),
                      temp_passwords.end(),
                      [](const auto &a,
                              const auto &b) -> bool {
                          return a.second < b.second;
                      });


            /// Restore the values
            element.second.passwords = std::map<std::size_t, std::string>(
                    temp_passwords.begin(),
                    temp_passwords.end());
        }
    }

    fmt::print("\n[+] Passwords sorted successfully\n");
}

/**
 * @brief Removes a password from either the password list or a specific category.
 * @param category The category object.
 */
auto passwords::remove(categories &category) -> void {
    int delete_option = read_input<int>("Delete Password From:\n[1] Password List\n"
                                   "[2] Category\nEnter your choice: ",
                                   "Invalid input. Please enter a valid option.",
                                   {1, 2});

    if (delete_option == 1) {
        /// Check if the password list is empty
        if (!is_printable()) {
            fmt::print("\n[-] No Passwords Found Inside of Password List");
            return;
        }

        /// Prompt the user to enter the ID of the password to delete
        auto password_id =
                read_input<std::size_t>("\nEnter the ID of the password to delete: ",
                                        "Invalid ID. Please enter a valid ID.",
                                        get_password_ids());

        /// Find the password with the given ID in the password list
        auto password_it = _pass_without_categories.find(password_id);

        if (password_it != _pass_without_categories.end()) {
            /// Delete the password from the password list
            _pass_without_categories.erase(password_it);
            fmt::print("\n[+] Password deleted successfully\n");
        } else fmt::print("\n[-] Password with ID {} not found\n", password_id);

    } else if (delete_option == 2) {
        /// Check if there are any categories available
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
            /// Attempt to convert the input to a size_t for ID
            std::size_t id = std::stoull(input);
            identifier = id;
        } catch (...) {
            /// If the conversion fails, assume the input is a category name
            identifier = input;
        }

        /// Get the selected category based on the identifier
        std::optional<categories::category> selected_category = category.get(identifier);
        if (selected_category.has_value()) {
            /// Prompt the user to enter the ID of the password
            /// to delete within the selected category
            auto password_id =
                    read_input<std::size_t>("Enter the ID of the password to delete: ",
                                            "Invalid ID. Please enter a valid ID.",
                                            {1, selected_category->_pass_id});

            if (password_id >= 1 && password_id <= selected_category->passwords.size()) {
                /// Find the category and delete the password within it
                auto category_it = category.categories_map.find(selected_category->ID);
                category_it->second.passwords.erase(
                        selected_category->passwords.find(password_id));
                fmt::print("\n[+] Password Deleted Successfully\n");

            } else fmt::print("\n[-] Invalid Password ID\n");
        } else fmt::print("\n[-] Category Not Found\n");
    }
}

/**
 * @brief Retrieves the IDs of all passwords in the list.
 * @return A vector containing the password IDs.
 */
auto passwords::get_password_ids() const -> std::vector<std::size_t> {
    /// Create a vector to store the password IDs
    std::vector<std::size_t> password_ids;
    password_ids.reserve(_pass_without_categories.size());

    /// Iterate over each password in the password list
    for (const auto &password : _pass_without_categories) {
        /// Add the password ID to the vector
        password_ids.push_back(password.first);
    }

    return password_ids;
}

/**
 * @brief Checks if the password list is printable.
 * @return True if the password list is not empty, false otherwise.
 */
auto passwords::is_printable() -> bool {
    /// Check if the password list is empty
    bool has_passwords = !_pass_without_categories.empty();

    /// If the password list is not empty, print the passwords
    if (has_passwords) {
        for (auto const &pass : _pass_without_categories) {
            fmt::print("\n[+] ID: {} Name: {}",
                       pass.second.ID, pass.second.name);
        }
    } else fmt::print("\n[-] No Passwords Found\n");

    return has_passwords;
}

/**
 * @brief Edit passwords either from the password list or from a specific category.
 * @param category The categories object used to access and modify categories and passwords.
 */
auto passwords::edit(categories &category) -> void {
    int edit_option = read_input<int>("Edit Password From:\n[1] Password List\n"
                                      "[2] Category\nEnter your choice: ",
                                      "Invalid input. Please enter a valid option.",
                                      {1, 2});

    /// Edit password from the password list
    if (edit_option == 1) {
        /// Check if the password list is printable
        if (!is_printable()) {
            fmt::print("\n[-] No Passwords Found Inside the Password List");
            return;
        }

        /// Prompt the user to enter the ID of the password to edit
        auto password_id =
                read_input<std::size_t>("\nEnter the ID of the password to edit: ",
                                        "Invalid ID. Please enter a valid ID.",
                                        get_password_ids());

        /// Find the password in the password list based on the ID
        auto password_it = _pass_without_categories.find(password_id);

        /// If the password is found
        if (password_it != _pass_without_categories.end()) {
            /// Initialize password struct by reference
            /// This does not creates any copies or whatsoever
            auto &password = password_it->second;

            fmt::print("\n[+] Editing Password ID: {}\n", password_id);

            /// Prompt the user to enter the new password
            std::string new_password;
            std::cout << "Enter the New Password: ";
            std::cin >> new_password;

            /// Check if the new password meets the security requirements
            if (is_secure(new_password)) {
                /// Update the password with the new value
                password.name = new_password;
                fmt::print("\n[+] Password Edited Successfully\n");

            } else fmt::print("\n[-] New Password is Not Secure. Please Try Again.\n");

        } else fmt::print("\n[-] Password with ID {} Not Found\n", password_id);

    } else if (edit_option == 2) {
        /// Check if any categories are printable (exist)
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

        /// Get the selected category based on the identifier
        std::optional<categories::category> selected_category = category.get(identifier);
        if (selected_category.has_value()) {
            /// Prompt the user to enter the ID of the password to edit
            auto password_id =
                    read_input<std::size_t>("Enter the ID of the password to edit: ",
                                            "Invalid ID. Please enter a valid ID.",
                                            {1, selected_category->passwords.size()});

            /// Check if the entered password ID is valid
            if (password_id >= 1 && password_id <= selected_category->passwords.size()) {
                auto category_it = category.categories_map.find(selected_category->ID);
                std::map<std::size_t, std::string> &passwords =
                        category_it->second.passwords;
                std::string &password = passwords[password_id];

                fmt::print("\n[+] Editing Password ID: {}\n", password_id);
                std::string new_password;
                std::cout << "Enter the New Password: ";
                std::cin >> new_password;

                /// Check if the new password meets the security requirements
                if (is_secure(new_password)) {
                    /// Update the password with the new value
                    password = new_password;
                    fmt::print("\n[+] Password Edited Successfully\n");

                } else fmt::print("\n[-] New Password is Not Secure. Please Try Again.\n");

            } else fmt::print("\n[-] Invalid Password ID\n");

        } else fmt::print("\n[-] Category Not Found\n");
    }
}
