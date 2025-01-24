#include <gtest/gtest.h>
#include <hash256.h>

// La clase puede instanciarse y cerrarse correctamente.
TEST(Hash256, Instanciar)
{
    Hash256 hasher{};
}

// La clase puede calcular el hash de una cadena de caracteres.
TEST(Hash256, TestCaclulateHash)
{
    const std::string input("HolaEstoEsUnTest");

    // Fuente: https://emn178.github.io/online-tools/sha256.html
    const std::string expected("2dc61c4df1a79838f548208438303ff07d154d7d2f9464c8cf308c59b6b49752");

    Hash256 hasher{};
    std::string ouput = hasher.calculateHex(input);
    ASSERT_EQ(ouput.length(), 64);
    ASSERT_EQ(ouput, expected);
}