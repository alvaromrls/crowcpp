#pragma once

#include "openssl/sha.h"
#include <openssl/evp.h>
#include "IHashCalculator.h"

#include <memory>

class Hash256 : IHashCalculator
{
private:
    std::string _hash;
    std::unique_ptr<EVP_MD_CTX, void (*)(EVP_MD_CTX *)> mdctx;

    // Método interno para calcular el hast de una cadena
    std::string calculateHash(const std::string &input);

public:
    Hash256();
    ~Hash256();

    Hash256(const Hash256 &) = delete;            // No copiable
    Hash256 &operator=(const Hash256 &) = delete; // No asignable

    // Método para calcular el hash SHA-256 de un texto (formato HEX)
    std::string calculateHex(const std::string &input) override;

    // Método para verificar si un hash coincide con el hash calculado de un texto
    bool verifyHexHash(const std::string &input, const std::string &hash) override;
};
