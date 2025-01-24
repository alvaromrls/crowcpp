#pragma once

#include "openssl/sha.h"
#include <openssl/evp.h>

#include <memory>
#include <string>

class Hash256
{
private:
    std::string _hash;
    std::unique_ptr<EVP_MD_CTX, void (*)(EVP_MD_CTX *)> mdctx;

    // Método para obtener el hash en formato hexadecimal
    std::string toHex(const std::string &hash);

    // Método interno para calcular el hast de una cadena
    std::string _calculateHash(const std::string &input);

public:
    Hash256();
    ~Hash256();

    Hash256(const Hash256 &) = delete;            // No copiable
    Hash256 &operator=(const Hash256 &) = delete; // No asignable

    // Método para calcular el hash SHA-256 de un texto (formato HEX)
    std::string calculateHex(const std::string &input);

    // Método para verificar si un hash coincide con el hash calculado de un texto
    bool verifyHexHash(const std::string &input, const std::string &hash);
};
