#pragma once
#include <string>
#include <vector>

enum class HashAlgorithm
{
    SHA256,
    SHA512,
    MD5,
    // Agrega más algoritmos según sea necesario
};

class IHashAdapter
{
public:
    virtual ~IHashAdapter() = default;

    virtual void initialize(HashAlgorithm algorithm) = 0;
    virtual void update(const std::string &input) = 0;
    virtual std::vector<unsigned char> finalize() = 0;
};
