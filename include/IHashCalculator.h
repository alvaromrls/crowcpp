#pragma once

#include <string>

class IHashCalculator
{
public:
    virtual ~IHashCalculator() = default;

    // Calcula el hash en formato hexadecimal
    virtual std::string calculateHex(const std::string &input) = 0;

    // Verifica si un hash coincide con el cálculo de la entrada
    virtual bool verifyHexHash(const std::string &input, const std::string &hash) = 0;
};
