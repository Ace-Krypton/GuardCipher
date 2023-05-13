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

    return 0;
}
