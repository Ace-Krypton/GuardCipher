/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

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
