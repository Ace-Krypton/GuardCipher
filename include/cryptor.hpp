/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#pragma once

#include "categories.hpp"

class cryptor {
public:
    static auto encrypt(const std::string &plaintext,
                 const std::string &key) -> std::string;
    static auto initialize_encrypt(categories &category) -> void;
    static auto encrypt_map(std::map<std::size_t, std::string> &passwords,
                            const std::string &encryption_key) -> void;
    static auto write(const categories &category,
                      const std::map<std::size_t, std::string> &data,
                      const std::string &filename) -> bool;

    [[maybe_unused]] static auto decrypt(const std::string &ciphertext,
                                         const std::string &key) -> std::string;

private:
    inline static std::string _secret_key;
};
