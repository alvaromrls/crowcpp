#include "HexUtils.h"

std::string HexUtils::toHex(const std::string &binary)
{
    std::stringstream ss;

    auto toHexChar = [&ss](unsigned char c)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(c);
    };

    std::for_each(binary.begin(), binary.end(), toHexChar);

    return ss.str();
}