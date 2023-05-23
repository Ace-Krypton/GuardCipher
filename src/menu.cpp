/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/menu.hpp"

/**
 * @brief Displays the menu for the Password Manager.
 * @param menu The vector of items to display in the menu.
 */
auto menu::display(const std::vector<item> &menu) -> void {
    fmt::print("\n==================================================|\n");
    fmt::print("               Password Manager                   |\n");
    fmt::print("==================================================|\n");
    /// Iterate over each item in the menu and print its ID and label
    for (const item &item : menu) {
        fmt::print("[{}] {}\n", item.id, item.label);
    }
    fmt::print("==================================================|\n");
}

/**
 * @brief Processes the menu options for the Password Manager.
 *
 * This function displays the menu recursively, allows the user to make a selection,
 * and performs the corresponding action based on the selected item. It continues
 * processing the menu until the user chooses to exit.
 *
 * @param password The passwords object for managing passwords.
 * @param category The categories object for managing categories.
 * @param menu The vector of items representing the menu.
 */
auto menu::process(passwords &password, categories &category, const std::vector<item> &menu) -> void {
    /// Termination flag
    std::atomic<bool> flag = true;

    /// Continue processing the menu until the flag is false
    while (flag.load()) {
        /// Used std::function to get rid of CLang recursive call warning
        std::function<void(const std::vector<item>&)> process_menu_recursive;
        process_menu_recursive = [&](const std::vector<item> &current_menu) -> void {
            /// Display the current menu
            display(current_menu);

            /// User input
            fmt::print("\n-> ");
            std::size_t choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                fmt::print("\n[-] Invalid Input, Try Again\n");
                return;
            }

            /// Find the selected item in the current menu
            std::optional<item> selected = find_item(current_menu, choice);
            if (selected) {
                /// If the selected item has a sub-menu, recursively process it
                if (!selected->sub_menu.empty()) process_menu_recursive(selected->sub_menu);

                /// If the selected item does not have a sub-menu, handle the option
                else handle_option(password, category, selected->id, flag);
            } else fmt::print("\n[-] Invalid Input, Try Again\n");
        };

        /// Start processing the menu from the top-level menu
        process_menu_recursive(menu);
    }
}

/**
 * @brief Finds an item in the menu by its ID.
 *
 * This function searches for an item in the provided menu vector based on its ID.
 * If a matching item is found, it returns an optional containing the item. Otherwise,
 * it returns an empty optional (std::nullopt).
 *
 * @param menu The vector of items representing the menu.
 * @param id The ID of the item to find.
 * @return An optional containing the found item, or an empty optional if not found.
 */
auto menu::find_item(const std::vector<item> &menu, std::size_t id) -> std::optional<menu::item> {
    /// Iterate over each item in the menu
    for (const item &item: menu) {
        /// Check if the item's ID matches the provided ID
        /// And return the matching item
        if (item.id == id) return item;
    }

    return std::nullopt;
}

/**
 * @brief Handles the selected option in the menu.
 *
 * This function takes the selected option ID and performs the corresponding action
 * based on the ID. It invokes the appropriate methods on the passwords and categories
 * objects to handle the selected option.
 *
 * @param password The passwords object for managing passwords.
 * @param category The categories object for managing categories.
 * @param option_ID The ID of the selected option.
 * @param flag The atomic flag indicating whether to continue processing the menu.
 */
auto menu::handle_option(passwords &password, categories &category,
                         std::size_t option_ID, std::atomic<bool> &flag) -> void {
    /// Perform the action based on the selected option ID
    switch (option_ID) {
        case 1: category.add(); break;
        case 2: category.remove(); break;
        case 3: category.is_printable(); break;
        case 4: password.search(category); break;
        case 5: password.sort(category); break;
        case 6: password.add(category); break;
        case 7: password.edit(category); break;
        case 8: password.remove(category); break;
        case 9: cryptor::initialize_encrypt(category); break;
        case 10: cryptor::initialize_decrypt(category); break;
        case 0: flag.store(false); break;
        default: fmt::print("\n[-] Invalid Input, Try Again\n");
    }
}
