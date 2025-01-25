#include "OpenSSLAdapter.h"
#include <stdexcept>
#include <openssl/evp.h>

OpenSSLAdapter::OpenSSLAdapter()
    : mdctx(nullptr, EVP_MD_CTX_free)
{
    // Crear el contexto de OpenSSL
    mdctx.reset(EVP_MD_CTX_new());
    if (!mdctx)
    {
        throw std::runtime_error("OpenSSLAdapter: Unable to create context");
    }
}

void OpenSSLAdapter::initialize(HashAlgorithm algorithm)
{
    const EVP_MD *md = nullptr;

    // Selección del algoritmo
    switch (algorithm)
    {
    case HashAlgorithm::SHA256:
        md = EVP_sha256();
        break;
    case HashAlgorithm::SHA512:
        md = EVP_sha512();
        break;
    case HashAlgorithm::MD5:
        md = EVP_md5();
        break;
    default:
        throw std::invalid_argument("OpenSSLAdapter: Unsupported algorithm");
    }

    // Inicialización del contexto con el algoritmo
    if (1 != EVP_DigestInit_ex(mdctx.get(), md, nullptr))
    {
        throw std::runtime_error("OpenSSLAdapter: Initialization failed");
    }
}

void OpenSSLAdapter::update(const std::string &input)
{
    if (1 != EVP_DigestUpdate(mdctx.get(), input.c_str(), input.size()))
    {
        throw std::runtime_error("OpenSSLAdapter: Update failed");
    }
}

std::vector<unsigned char> OpenSSLAdapter::finalize()
{
    std::vector<unsigned char> buffer(EVP_MAX_MD_SIZE);
    unsigned int len = 0;

    if (1 != EVP_DigestFinal_ex(mdctx.get(), buffer.data(), &len))
    {
        throw std::runtime_error("OpenSSLAdapter: Finalize failed");
    }

    buffer.resize(len);
    return buffer;
}
