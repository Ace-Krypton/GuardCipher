/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include <vector>
#include <functional>

#include "../include/AddCategory.hpp"

struct MenuItem {
    std::size_t id;
    std::string label;
    std::vector<MenuItem> sub_menu;
};

auto display_menu(const std::vector<MenuItem> &menu) -> void {
    for (const MenuItem &item : menu) {
        std::cout << item.id << ". " << item.label << std::endl;
    }
}

auto find_menu_item(const std::vector<MenuItem> &menu, std::size_t id) -> const MenuItem* {
    for (const MenuItem &item : menu) {
        if (item.id == id) return &item;
    }
    return nullptr;
}

auto process_menu(const std::vector<MenuItem> &menu) -> void {
    std::function<void(const std::vector<MenuItem>&)> process_menu_recursive =
            [&](const std::vector<MenuItem>& current_menu) -> void {
                display_menu(current_menu);

        std::cout << "\n-> ";
        std::size_t choice;
        std::cin >> choice;

        const MenuItem* selected = find_menu_item(current_menu, choice);
        if (selected) {
            if (!selected->sub_menu.empty()) {
                process_menu_recursive(selected->sub_menu);
            } else {
                switch (selected->id) {
                    case 1: {
                        AddCategory category;
                        category.add_category("WhatsApp");
                        category.add_category("Instagram");
                        category.add_category("Facebook");

                        std::optional<AddCategory::Category> category_by_name =
                                category.get_category_by_name("WhatsApp");

                        if (category_by_name)
                            fmt::print("\n[+] ID: {}, Name: {}", category_by_name->id, category_by_name->category_name);
                        else
                            fmt::print("\n[-] Category Not Found");
                        break;
                    }

                    case 0:
                        return;

                    default:
                        std::cout << "Invalid Input, Try Again\n" << std::endl;
                }
            }
        } else {
            std::cout << "Invalid Input, Try Again\n" << std::endl;
        }
    };

    process_menu_recursive(menu);
}

auto main() -> int {
    std::vector<MenuItem> menu {
            {1, "Main Menu", {
                    {1, "Add Category"},
                    {0, "Exit"}
            }}
    };

    process_menu(menu);

    return 0;
}
