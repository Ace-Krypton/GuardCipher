/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/menu.hpp"

auto menu::display(const std::vector<item> &menu) -> void {
    fmt::print("\n==================================================|\n");
    fmt::print("               Password Manager                   |\n");
    fmt::print("==================================================|\n");
    for (const item &item : menu) {
        fmt::print("[{}] {}\n", item.id, item.label);
    }
    fmt::print("==================================================|\n");
}

auto menu::process(passwords &password, categories &category, const std::vector<item> &menu) -> void {
    std::atomic<bool> flag = true;
    while (flag.load()) {
        std::function<void(const std::vector<item>&)> process_menu_recursive;
        process_menu_recursive = [&](const std::vector<item> &current_menu) -> void {
            display(current_menu);

            fmt::print("\n-> ");
            std::size_t choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                fmt::print("\n[-] Invalid Input, Try Again\n");
                return;
            }

            std::optional<item> selected = find_item(current_menu, choice);
            if (selected) {
                if (!selected->sub_menu.empty()) process_menu_recursive(selected->sub_menu);
                else handle_option(password, category, selected->id, flag);
            } else fmt::print("\n[-] Invalid Input, Try Again\n");
        };
        process_menu_recursive(menu);
    }
}

auto menu::find_item(const std::vector<item> &menu, std::size_t id) -> std::optional<menu::item> {
    for (const item &item : menu) {
        if (item.id == id) return item;
    }
    return std::nullopt;
}

auto menu::handle_option(passwords &password, categories &category,
                         std::size_t option_ID, std::atomic<bool> &flag) -> void {
    switch (option_ID) {
        case 1: category.add(); break;
        case 2: category.remove(); break;
        case 3: password.add(category); break;
        case 4: category.print(); break;
        case 0: flag.store(false); break;
        default: fmt::print("\n[-] Invalid Input, Try Again\n");
    }
}
