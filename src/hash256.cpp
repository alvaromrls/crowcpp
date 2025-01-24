#include "hash256.h"

#include <sstream>
#include <iomanip>
#include <algorithm>

std::string Hash256::toHex(const std::string &hash)
{
    std::stringstream ss;

    auto toHex = [&ss](unsigned char c)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)c;
    };

    // Aplica la lambda a cada carácter de la cadena
    std::for_each(hash.begin(), hash.end(), toHex);

    return ss.str();
}

Hash256::Hash256()
    : mdctx(nullptr, EVP_MD_CTX_free) // Inicialización con nullptr y el deleter
{
    mdctx.reset(EVP_MD_CTX_new()); // Asignación real del recurso
    if (!mdctx)
    {
        throw std::runtime_error("Unable to create EVP context");
    }
}

Hash256::~Hash256()
{
}

std::string Hash256::_calculateHash(const std::string &input)
{

    if (1 != EVP_DigestInit_ex(mdctx.get(), EVP_sha256(), nullptr))
    {
        throw std::runtime_error("Unable to initialize Digest");
    }

    if (1 != EVP_DigestUpdate(mdctx.get(), input.c_str(), input.length()))
    {
        throw std::runtime_error("Unable to update Digest");
    }

    std::vector<unsigned char> buffer(EVP_MAX_MD_SIZE);
    unsigned int len;
    if (1 != EVP_DigestFinal_ex(mdctx.get(), buffer.data(), &len))
    {
        throw std::runtime_error("Unable to finalize Digest");
    }

    buffer.resize(len);
    return std::string(buffer.begin(), buffer.begin() + len);
}

std::string Hash256::calculateHex(const std::string &input)
{
    return toHex(_calculateHash(input));
}
