/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/Menu.hpp"

auto Menu::display_menu(const std::vector<MenuItem> &menu) -> void {
    fmt::print("\n==================================================|\n");
    fmt::print("               Password Manager                   |\n");
    fmt::print("==================================================|\n");
    for (const MenuItem &item : menu) {
        fmt::print("[{}] {}\n", item.id, item.label);
    }
    fmt::print("==================================================|\n");
}

auto Menu::process_menu(const std::vector<MenuItem> &menu) -> void {
    std::atomic<bool> flag = true;
    while (flag.load()) {
        std::function<void(const std::vector<MenuItem>&)> process_menu_recursive;
        process_menu_recursive = [&](const std::vector<MenuItem> &current_menu) -> void {
            display_menu(current_menu);

            fmt::print("\n-> ");
            std::size_t choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                fmt::print("\n[-] Invalid Input, Try Again\n");
                return;
            }

            std::optional<MenuItem> selected = find_menu_item(current_menu, choice);
            if (selected) {
                if (!selected->sub_menu.empty()) process_menu_recursive(selected->sub_menu);
                else handle_menu_option(selected->id, flag);
            } else fmt::print("\n[-] Invalid Input, Try Again\n");
        };
        process_menu_recursive(menu);
    }
}

auto Menu::find_menu_item(const std::vector<MenuItem> &menu, std::size_t id) -> std::optional<Menu::MenuItem> {
    for (const MenuItem &item : menu) {
        if (item.id == id) return item;
    }
    return std::nullopt;
}

auto Menu::handle_menu_option(std::size_t option_ID, std::atomic<bool> &flag) -> void {
    switch (option_ID) {
        case 1: {
            Categories category;
            category.add_category("WhatsApp");
            category.add_category("Instagram");
            category.add_category("Facebook");

            std::optional<Categories::Category> category_by_name =
                    category.get_category_by_name("WhatsApp");

            if (category_by_name) fmt::print("\n[+] ID: {}, Name: {}",
                                             category_by_name->id, category_by_name->category_name);
            else fmt::print("\n[-] Category Not Found");
            break;
        }

        case 0:
            flag.store(false);
            break;

        default:
            fmt::print("\n[-] Invalid Input, Try Again\n");
    }
}
