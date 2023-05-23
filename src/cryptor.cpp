/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include <fstream>
#include "../include/cryptor.hpp"

auto cryptor::encrypt(const std::string &plaintext,
                      const std::string &key) -> std::string {
    std::size_t key_index = 0;
    std::string cipher_text = plaintext;

    for (char &i : cipher_text) {
        i = static_cast<char>((static_cast<unsigned char>(i)
                               + static_cast<unsigned char>(key[key_index])) % 256);
        i = static_cast<char>(static_cast<unsigned char>(i)
                              ^ (static_cast<unsigned char>(key[key_index]) & 0xFF));
        key_index = (key_index + 1) % key.length();
    }

    return cipher_text;
}

auto cryptor::decrypt(const std::string &ciphertext,
                      const std::string &key) -> std::string {
    std::size_t key_index = 0;
    std::string plain_text = ciphertext;

    for (char &i : plain_text) {
        i = static_cast<char>(static_cast<unsigned char>(i)
                              ^ (static_cast<unsigned char>(key[key_index]) & 0xFF));
        i = static_cast<char>((static_cast<unsigned char>(i)
                               - static_cast<unsigned char>(key[key_index]) + 256) % 256);
        key_index = (key_index + 1) % key.length();
    }

    return plain_text;
}

auto cryptor::encrypt_map(std::map<std::size_t, std::string> &passwords,
                          const std::string &encryption_key) -> void {
    for (auto& [key, value] : passwords) {
        value = encrypt(value, encryption_key);
    }
}

auto cryptor::initialize_encrypt(categories &category) -> void {
    if (!category.is_printable()) return;
    fmt::print("Enter the secret key: ");

    std::string key;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, key);

    _secret_key = std::move(key);

    for (auto& [category_ID, _category] : category.categories_map) {
        encrypt_map(_category.passwords, _secret_key);
        write(category, _category.passwords, "encrypted_map.txt");
    }

    fmt::print("[+] All Data Encrypted Successfully\n");
}

auto cryptor::write(const categories &category,
                    const std::map<std::size_t, std::string> &data,
                    const std::string &filename) -> bool {
    std::ofstream file(filename);

    if (!file) {
        fmt::print("[-] Failed to Open the File '{}'", filename);
        return false;
    }

    file << "\n----------- Categories -----------\n";
    for (const auto &element : category.categories_map) {
        file << "[+] ID: " << element.second.ID <<
             " Name: " << element.second.name << '\n' << " Passwords:\n";
        for (const auto& [key, value] : data) {
            file << "ID: " << key << " Pass: " << value << '\n';
        }
        file << '\n';
    }

    file.close();

    fmt::print("[+] Map data written to file '{}'\n", filename);
    return true;
}
