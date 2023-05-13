/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include "../include/AddCategory.hpp"

auto main() -> int {
    AddCategory category;
    category.add_category("WhatsApp");
    category.add_category("Instagram");
    category.add_category("Facebook");

    std::optional<AddCategory::Category> category_by_ID = category.get_category_by_ID(2);
    if (category_by_ID) {
        std::cout << "Category ID: " << category_by_ID->id << ", Name: " << category_by_ID->category_name << std::endl;
    } else {
        std::cout << "Category not found." << std::endl;
    }

    std::optional<AddCategory::Category> category_by_name = category.get_category_by_name("Instagram");
    if (category_by_name) {
        std::cout << "Category ID: " << category_by_name->id << ", Name: " << category_by_name->category_name << std::endl;
    } else {
        std::cout << "Category not found." << std::endl;
    }

    std::variant<std::uint32_t, std::string> identifier1 = 1U;
    std::optional<AddCategory::Category> category1 = category.get_category(identifier1);
    if (category1) {
        std::cout << "Category ID: " << category1->id << ", Name: " << category1->category_name << std::endl;
    } else {
        std::cout << "Category not found." << std::endl;
    }

    std::variant<std::uint32_t, std::string> identifier2 = "WhatsApp";
    std::optional<AddCategory::Category> category2 = category.get_category(identifier2);
    if (category2) {
        std::cout << "Category ID: " << category2->id << ", Name: " << category2->category_name << std::endl;
    } else {
        std::cout << "Category not found." << std::endl;
    }

    return 0;
}
