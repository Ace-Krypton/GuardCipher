/*
 * (C)opyright 2023 Ramiz Abbasov <ramizna@code.edu.az>
 * See LICENSE file for license details
 */

#pragma once

#include <string>

class cryptor {
    auto encrypt(const std::string &plaintext,
                 const std::string &key) -> std::string;
    auto decrypt(const std::string &ciphertext,
                 const std::string &key) -> std::string;
};
