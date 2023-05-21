/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/categories.hpp"

/**
 * @brief Adds a new category.
 *
 * This function prompts the user to enter a category name and adds a new category
 * with an assigned ID to the categories_map. It then prints a success message.
 */
auto categories::add() -> void {
    /// Create a new category struct
    struct category new_category;

    /// Prompt the user to enter the category name
    fmt::print("Enter the Category Name: ");
    std::string category_name;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, category_name);

    /// Assign the category name and ID
    new_category.name = category_name;
    new_category.ID = _current_ID++;

    /// Add the new category to the categories_map
    categories_map[new_category.ID] = new_category;
    fmt::print("\n[+] Category Added Successfully\n");
}

/**
 * @brief Retrieves a category by its ID.
 *
 * This function looks up the category with the specified ID in the categories_map.
 * If a matching category is found, it returns an optional containing the category.
 * Otherwise, it returns an empty optional (std::nullopt).
 *
 * @param category_ID The ID of the category to retrieve.
 * @return An optional containing the found category, or an empty optional if not found.
 */
auto categories::get_ID(std::size_t category_ID) const -> std::optional<category> {
    /// Find the category in the categories_map using its ID
    auto it = categories_map.find(category_ID);

    /// Check if the category is found
    /// If found return the found category
    if (it != categories_map.end()) return it->second;

    return std::nullopt;
}

/**
 * @brief Retrieves a category by its name.
 *
 * This function searches for a category with the specified name in the categories_map.
 * If a matching category is found, it returns an optional containing the category.
 * Otherwise, it returns an empty optional (std::nullopt).
 *
 * @param category_name The name of the category to retrieve.
 * @return An optional containing the found category, or an empty optional if not found.
 */
auto categories::get_name(const std::string &category_name) const -> std::optional<category> {
    /// Iterate over each element in the categories_map
    for (const auto &element : categories_map) {
        /// Check if the category name matches the specified name
        if (element.second.name == category_name) {
            /// Return the found category
            return element.second;
        }
    }
    return std::nullopt;
}

/**
 * @brief Retrieves a category by its ID or name.
 *
 * This function allows retrieving a category by either its ID or name. It takes
 * a variant parameter that can hold either a size_t representing the category ID
 * or a string representing the category name. It uses the appropriate helper
 * functions, get_ID() or get_name(), to perform the retrieval based on the variant value.
 *
 * @param identifier The variant identifier containing either the category ID or name.
 * @return An optional containing the found category, or an empty optional if not found.
 */
auto categories::get(const std::variant<std::size_t,
                     std::string> &identifier) const -> std::optional<category> {
    /// Check if the identifier holds a size_t value (category ID)
    if (std::holds_alternative<std::size_t>(identifier)) {
        /// Retrieve the category ID from the identifier
        std::size_t category_ID = std::get<std::size_t>(identifier);
        /// Use the get_ID() function to retrieve the category by ID
        return get_ID(category_ID);
    }

    /// Check if the identifier holds a string value (category name)
    else if (std::holds_alternative<std::string>(identifier)) {
        /// Retrieve the category name from the identifier
        std::string category_name = std::get<std::string>(identifier);
        /// Use the get_name() function to retrieve the category by name
        return get_name(category_name);
    }

    return std::nullopt;
}

/**
 * @brief Checks if there are printable categories.
 *
 * This function checks if there are any categories in the categories_map.
 * If there are categories present, it prints the ID, name, and passwords
 * for each category. If no categories are found, it prints a corresponding
 * message. The function returns a boolean value indicating whether there
 * are printable categories or not.
 *
 * @return True if there are printable categories, false otherwise.
 */
auto categories::is_printable() -> bool {
    /// Check if there are categories in the categories_map
    bool has_categories = !categories_map.empty();

    /// If there are categories, print their details
    if (has_categories) {
        fmt::print("\n----------- Categories -----------\n");
        for (const auto &category : categories_map) {
            fmt::print("\n[+] ID: {} Name: {}\n Passwords:\n",
                       category.second.ID, category.second.name);
            for (const auto &password : category.second.passwords) {
                fmt::print("ID: {}, {}\n", password.first, password.second);
            }
        }
    } else fmt::print("\n[-] No Category Found\n");

    return has_categories;
}

/**
 * @brief Removes a category.
 *
 * This function prompts the user to choose a category to delete,
 * retrieves the user's input, and uses the get() function to find
 * the corresponding category. If the category is found, it confirms
 * with the user before deleting the category from the categories_map.
 * Otherwise, it displays a message indicating that the category was not found.
 */
auto categories::remove() -> void {
    if (!is_printable()) return;
    fmt::print("Choose Category to Delete: ");

    /// Prompt user to choose a category for deletion
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    /// Convert user input into a variant representing category ID or name
    std::variant<std::size_t, std::string> identifier;
    try {
        std::size_t id = std::stoull(input);
        identifier = id;
    } catch (...) {
        identifier = input;
    }

    /// Retrieve the selected category using the get() function
    std::optional<category> category_selected = get(identifier);
    if (category_selected.has_value()) {
        fmt::print("Are You Sure You Want to Delete the Category '{}'? (Y/N): ",
                   category_selected->name);
        /// Confirm deletion with the user
        std::string confirmation;
        std::cin >> confirmation;
        if (confirmation.size() == 1 && std::toupper(confirmation[0]) == 'Y') {
            /// Remove the category from the categories_map
            categories_map.erase(category_selected->ID);
            fmt::print("\n[+] Category Deleted Successfully\n");
        } else fmt::print("\n[-] Canceled\n");

    } else fmt::print("\n[-] Category Not Found\n");
}
