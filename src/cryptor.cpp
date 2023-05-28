/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include <fstream>
#include "../include/cryptor.hpp"

/**
 * @brief Encrypts the plaintext using the provided key.
 *
 * @param plaintext The plaintext to encrypt.
 * @param key       The encryption key.
 * @return The encrypted ciphertext.
 */
auto cryptor::encrypt(const std::string &plaintext,
                      const std::string &key) -> std::string {
    std::size_t key_index = 0;
    std::string cipher_text = plaintext;

    for (char &i : cipher_text) {
        /// Add the key value to the plaintext character and take modulo 256
        i = static_cast<char>((static_cast<unsigned char>(i)
                               + static_cast<unsigned char>(key[key_index])) % 256);
        /// XOR the result with the lower 8 bits of the key value
        i = static_cast<char>(static_cast<unsigned char>(i)
                              ^ (static_cast<unsigned char>(key[key_index]) & 0xFF));
        key_index = (key_index + 1) % key.length();
    }

    return cipher_text;
}

/**
 * @brief Decrypts the ciphertext using the provided key.
 *
 * @param ciphertext The ciphertext to decrypt.
 * @param key        The encryption key.
 * @return The decrypted plaintext.
 */
[[maybe_unused]] auto cryptor::decrypt(const std::string &ciphertext,
                      const std::string &key) -> std::string {
    std::size_t key_index = 0;
    std::string plain_text = ciphertext;

    for (char &i : plain_text) {
        /// XOR the ciphertext character with the lower 8 bits of the key value
        i = static_cast<char>(static_cast<unsigned char>(i)
                              ^ (static_cast<unsigned char>(key[key_index]) & 0xFF));
        /// Subtract the key value from the result and add 256, then take modulo 256
        i = static_cast<char>((static_cast<unsigned char>(i)
                               - static_cast<unsigned char>(key[key_index]) + 256) % 256);
        key_index = (key_index + 1) % key.length();
    }

    return plain_text;
}

/**
 * @brief Encrypts the passwords in a map using the provided encryption key.
 *
 * @param passwords        The map of passwords to encrypt.
 * @param encryption_key   The encryption key.
 */
auto cryptor::encrypt_map(std::map<std::size_t, std::string> &passwords,
                          const std::string &encryption_key) -> void {
    for (auto& [key, value] : passwords) {
        value = encrypt(value, encryption_key);
    }
}

/**
 * @brief Initializes encryption for a given category.
 *        Encrypts the passwords in the category map and writes the encrypted data to a file.
 *
 * @param category The category to initialize encryption for.
 */
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

/**
 * @brief Writes the category and password data to a file.
 *
 * @param category The category object.
 * @param data     The map of password data.
 * @param filename The name of the file to write to.
 * @return True if the write operation was successful, false otherwise.
 */
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