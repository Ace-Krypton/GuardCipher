/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#include <fstream>
#include <sstream>
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
    std::cin >> key;
    _secret_key = std::move(key);

    for (auto& [category_ID, _category] : category.categories_map) {
        encrypt_map(_category.passwords, _secret_key);
        write(category, _category.passwords, "encrypted_map.txt");
    }

    fmt::print("[+] All Data Encrypted Successfully\n");
}

auto cryptor::initialize_decrypt(categories& category) -> void {
    std::string encryptedDataFilename = "encrypted_map.txt";

    // Read the encrypted data from the file
    std::map<std::size_t, std::string> encryptedData = read_encrypted_data(encryptedDataFilename);

    if (encryptedData.empty()) {
        fmt::print("[-] No encrypted data found.\n");
        return;
    }

    fmt::print("Enter the decryption _key: ");
    std::string _key;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, _key);

    _secret_key = std::move(_key);

    try {
        // Decrypt the passwords
        decrypt_map(encryptedData, _secret_key);

        // Update the decrypted passwords in the category map
        for (auto& [category_ID, _category] : category.categories_map) {
            for (auto& [key, value] : _category.passwords) {
                if (encryptedData.count(key) > 0) {
                    std::cout << "came here";
                    value = encryptedData[key];
                }
            }
        }

        fmt::print("[+] All Data Decrypted Successfully\n");
    } catch (const std::exception& e) {
        fmt::print("[-] Decryption error: {}\n", e.what());
    }
}

auto cryptor::read_encrypted_data(const std::string& filename) -> std::map<std::size_t, std::string> {
    std::ifstream file(filename);
    std::map<std::size_t, std::string> encryptedData;

    if (!file) {
        fmt::print("[-] Failed to open the file '{}'\n", filename);
        return encryptedData;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.find("ID: ") != std::string::npos && line.find(" Pass: ") != std::string::npos) {
            std::string id_str = line.substr(4, line.find(" Pass: ") - 4);
            std::string pass_str = line.substr(line.find(" Pass: ") + 8);

            try {
                std::size_t id = std::stoull(id_str);
                encryptedData[id] = pass_str;
            } catch (const std::exception& e) {
                fmt::print("[-] Invalid ID found: {}\n", id_str);
            }
        }
    }

    file.close();

    return encryptedData;
}

auto cryptor::decrypt_map(std::map<std::size_t, std::string>& passwords,
                          const std::string& decryption_key) -> void {
    for (auto& [key, value] : passwords) {
        value = decrypt(value, decryption_key);
    }
}

auto cryptor::write(const categories& category, const std::map<std::size_t,
                    std::string>& data, const std::string& filename) -> bool {
    std::ofstream file(filename);

    if (!file) {
        fmt::print("[-] Failed to open the file '{}'\n", filename);
        return false;
    }

    file << "\n----------- Categories -----------\n";

    for (const auto& element : category.categories_map) {
        file << "[+] ID: " << element.second.ID << " Name: " << element.second.name << '\n';
        file << "Passwords:\n";

        for (const auto& [key, value] : data) {
            file << "ID: " << key << " Pass: " << value << '\n';
        }
        file << '\n';
    }

    file.close();

    fmt::print("[+] Map data written to file '{}'\n", filename);
    return true;
}
