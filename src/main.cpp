/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include <string>
#include <variant>
#include <optional>
#include <iostream>
#include <unordered_map>

class CategoryManager {
public:
    struct Category {
        int id { };
        std::string name;
    };

    std::unordered_map<int, Category> categories;
    int currentID = 1;

public:
    void addCategory(const std::string& categoryName) {
        Category newCategory;
        newCategory.name = categoryName;
        newCategory.id = currentID++;

        categories[newCategory.id] = newCategory;
    }

    auto getCategoryByID(int categoryID) const -> std::optional<Category> {
        auto it = categories.find(categoryID);
        if (it != categories.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    std::optional<Category> getCategoryByName(const std::string& categoryName) const {
        for (const auto& pair : categories) {
            if (pair.second.name == categoryName) {
                return pair.second;
            }
        }
        return std::nullopt;
    }

    std::optional<Category> getCategory(const std::variant<int, std::string>& identifier) const {
        if (std::holds_alternative<int>(identifier)) {
            int categoryID = std::get<int>(identifier);
            return getCategoryByID(categoryID);
        } else if (std::holds_alternative<std::string>(identifier)) {
            std::string categoryName = std::get<std::string>(identifier);
            return getCategoryByName(categoryName);
        }
        return std::nullopt;
    }
};

auto main() -> int {
    CategoryManager manager;
    manager.addCategory("WhatsApp");
    manager.addCategory("Instagram");
    manager.addCategory("Facebook");

    // Get category by ID
    std::optional<CategoryManager::Category> categoryByID = manager.getCategoryByID(2);
    if (categoryByID) {
        std::cout << "Category ID: " << categoryByID->id << ", Name: " << categoryByID->name << std::endl;
    } else {
        std::cout << "Category not found." << std::endl;
    }

    // Get category by name
    std::optional<CategoryManager::Category> categoryByName = manager.getCategoryByName("Instagram");
    if (categoryByName) {
        std::cout << "Category ID: " << categoryByName->id << ", Name: " << categoryByName->name << std::endl;
    } else {
        std::cout << "Category not found." << std::endl;
    }

    // Get category using variant identifier
    std::variant<int, std::string> identifier1 = 3;
    std::optional<CategoryManager::Category> category1 = manager.getCategory(identifier1);
    if (category1) {
        std::cout << "Category ID: " << category1->id << ", Name: " << category1->name << std::endl;
    } else {
        std::cout << "Category not found." << std::endl;
    }

    std::variant<int, std::string> identifier2 = "WhatsApp";
    std::optional<CategoryManager::Category> category2 = manager.getCategory(identifier2);
    if (category2) {
        std::cout << "Category ID: " << category2->id << ", Name: " << category2->name << std::endl;
    } else {
        std::cout << "Category not found." << std::endl;
    }

    return 0;
}
