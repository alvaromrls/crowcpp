#pragma once

#include "IHashAdapter.h"
#include <openssl/evp.h>
#include <memory>

class OpenSSLAdapter : public IHashAdapter
{
private:
    std::unique_ptr<EVP_MD_CTX, void (*)(EVP_MD_CTX *)> mdctx;

public:
    // Constructor
    OpenSSLAdapter();

    // Métodos de la interfaz IHashAdapter
    void initialize(HashAlgorithm algorithm) override;
    void update(const std::string &input) override;
    std::vector<unsigned char> finalize() override;
};
