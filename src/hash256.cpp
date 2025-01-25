#include "hash256.h"
#include "HexUtils.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

Hash256::Hash256(std::shared_ptr<IHashAdapter> adapter)
    : hashAdapter(std::move(adapter))
{
}

std::string Hash256::calculateHash(const std::string &input)
{
    hashAdapter->initialize(HashAlgorithm::SHA256);
    hashAdapter->update(input);
    auto binaryHash = hashAdapter->finalize();
    return std::string(binaryHash.begin(), binaryHash.end());
}

std::string Hash256::calculateHex(const std::string &input)
{
    return HexUtils::toHex(calculateHash(input));
}

bool Hash256::verifyHexHash(const std::string &input, const std::string &hash)
{
    return hash == calculateHex(input);
}