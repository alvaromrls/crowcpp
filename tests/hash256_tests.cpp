#include <gtest/gtest.h>
#include <hash256.h>
#include "OpenSSLAdapter.h"
#include <gmock/gmock.h>

// La clase puede instanciarse y cerrarse correctamente.
TEST(Hash256, Instanciar)
{
    // Crear una instancia de OpenSSLAdapter
    auto opensslAdapter = std::make_shared<OpenSSLAdapter>();

    // Pasar el adaptador a Hash256
    Hash256 hasher(opensslAdapter);
}

// La clase puede calcular el hash de una cadena de caracteres.
TEST(Hash256, TestCaclulateHash)
{
    const std::string input("HolaEstoEsUnTest");

    // Fuente: https://emn178.github.io/online-tools/sha256.html
    const std::string expected("2dc61c4df1a79838f548208438303ff07d154d7d2f9464c8cf308c59b6b49752");

    // Crear una instancia de OpenSSLAdapter
    auto opensslAdapter = std::make_shared<OpenSSLAdapter>();

    // Pasar el adaptador a Hash256
    Hash256 hasher(opensslAdapter);
    std::string ouput = hasher.calculateHex(input);
    ASSERT_EQ(ouput.length(), 64);
    ASSERT_EQ(ouput, expected);
}

// La el metodo verifyHexHash devuelve true si un input y su hash coinciden
TEST(Hash256, TestValidateHash)
{
    const std::string input("HolaEstoEsUnTest");

    // Fuente: https://emn178.github.io/online-tools/sha256.html
    const std::string expected("2dc61c4df1a79838f548208438303ff07d154d7d2f9464c8cf308c59b6b49752");

    // Crear una instancia de OpenSSLAdapter
    auto opensslAdapter = std::make_shared<OpenSSLAdapter>();

    // Pasar el adaptador a Hash256
    Hash256 hasher(opensslAdapter);
    ASSERT_TRUE(hasher.verifyHexHash(input, expected));

    const std::string input2("validation");
    const std::string output2 = hasher.calculateHex(input2);
    ASSERT_TRUE(hasher.verifyHexHash(input2, output2));
}

// La el metodo verifyHexHash devuelve true si un input y su hash coinciden
TEST(Hash256, TestValidateWrongHash)
{
    const std::string input("HolaEstoEsUnTes");

    // Fuente: https://emn178.github.io/online-tools/sha256.html
    const std::string expected("2dc61c4df1a79838f548208438303ff07d154d7d2f9464c8cf308c59b6b49752");

    // Crear una instancia de OpenSSLAdapter
    auto opensslAdapter = std::make_shared<OpenSSLAdapter>();

    // Pasar el adaptador a Hash256
    Hash256 hasher(opensslAdapter);

    ASSERT_FALSE(hasher.verifyHexHash(input, expected));

    std::string input2("validation");
    const std::string output2 = hasher.calculateHex(input2);
    input2 += "a";
    ASSERT_FALSE(hasher.verifyHexHash(input2, output2));
}

// Calcular hash de una cadena vacía.
TEST(Hash256, TestEmptyStringHash)
{
    const std::string input("");
    const std::string expected("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");

    // Crear una instancia de OpenSSLAdapter
    auto opensslAdapter = std::make_shared<OpenSSLAdapter>();

    // Pasar el adaptador a Hash256
    Hash256 hasher(opensslAdapter);

    std::string output = hasher.calculateHex(input);
    ASSERT_EQ(output, expected);
}

// Calcular hash de una cadena con caracteres especiales.
TEST(Hash256, TestSpecialCharactersHash)
{
    const std::string input("!@#$%^&*()_+-={}[]:\\\";'<>?,./");
    const std::string expected("1316e4cbbdfdc5a249e12b3d2341c095a9485d9b0c6dc052991a9f598999728b");

    // Crear una instancia de OpenSSLAdapter
    auto opensslAdapter = std::make_shared<OpenSSLAdapter>();

    // Pasar el adaptador a Hash256
    Hash256 hasher(opensslAdapter);
    std::string output = hasher.calculateHex(input);
    ASSERT_EQ(output, expected);
}

// Calcular hash de una cadena UTF-8.
TEST(Hash256, TestUtf8Hash)
{
    const std::string input("こんにちは世界"); // "Hello, World" en japonés.
    const std::string expected("c6a304536826fb57e1b1896fcd8c91693a746233ae6a286dc85a65c8ae1f416f");

    // Crear una instancia de OpenSSLAdapter
    auto opensslAdapter = std::make_shared<OpenSSLAdapter>();

    // Pasar el adaptador a Hash256
    Hash256 hasher(opensslAdapter);

    std::string output = hasher.calculateHex(input);
    ASSERT_EQ(output, expected);
    ASSERT_TRUE(hasher.verifyHexHash(input, output));
}

// Calcular hash de una cadena extremadamente larga.
TEST(Hash256, TestLongStringHash)
{
    std::string input(10'000, 'a'); // 10,000 caracteres 'a'.

    // Crear una instancia de OpenSSLAdapter
    auto opensslAdapter = std::make_shared<OpenSSLAdapter>();

    // Pasar el adaptador a Hash256
    Hash256 hasher(opensslAdapter);

    ASSERT_NO_THROW({
        std::string output = hasher.calculateHex(input);
        ASSERT_EQ(output.length(), 64);
    });
}

// Verificar hashes malformados.
TEST(Hash256, TestMalformedHash)
{
    const std::string input("HolaEstoEsUnTest");

    // Crear una instancia de OpenSSLAdapter
    auto opensslAdapter = std::make_shared<OpenSSLAdapter>();

    // Pasar el adaptador a Hash256
    Hash256 hasher(opensslAdapter);

    // Hash demasiado corto.
    ASSERT_FALSE(hasher.verifyHexHash(input, "2dc61c4d"));

    // Hash con caracteres no válidos.
    ASSERT_FALSE(hasher.verifyHexHash(input, "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"));
}

class MockOpenSSLAdapter : public IHashAdapter
{
public:
    MOCK_METHOD(void, initialize, (HashAlgorithm algorithm), (override));
    MOCK_METHOD(void, update, (const std::string &input), (override));
    MOCK_METHOD(std::vector<unsigned char>, finalize, (), (override));
};

// La clase debería manejar correctamente errores de inicialización.
TEST(Hash256, TestInitializationError)
{
    const std::string input("HolaEstoEsUnTest");

    // Crear un mock de OpenSSLAdapter
    auto mockAdapter = std::make_shared<MockOpenSSLAdapter>();

    // Establecer expectativas para simular un error en la inicialización
    EXPECT_CALL(*mockAdapter, initialize(HashAlgorithm::SHA256)).WillOnce(testing::Throw(std::runtime_error("Initialization failed")));

    // Pasar el adaptador a Hash256
    Hash256 hasher(mockAdapter);

    // Verificar que la excepción sea lanzada
    EXPECT_THROW(hasher.calculateHex(input), std::runtime_error);
}

// La clase debería manejar correctamente errores durante la actualización.
TEST(Hash256, TestUpdateError)
{
    const std::string input("HolaEstoEsUnTest");

    // Crear un mock de OpenSSLAdapter
    auto mockAdapter = std::make_shared<MockOpenSSLAdapter>();

    // Establecer expectativas para simular un error durante la actualización
    EXPECT_CALL(*mockAdapter, initialize(HashAlgorithm::SHA256)).Times(1);
    EXPECT_CALL(*mockAdapter, update(input)).WillOnce(testing::Throw(std::runtime_error("Update failed")));

    // Pasar el adaptador a Hash256
    Hash256 hasher(mockAdapter);

    // Verificar que la excepción sea lanzada
    EXPECT_THROW(hasher.calculateHex(input), std::runtime_error);
}

// La clase debería manejar correctamente errores durante la finalización.
TEST(Hash256, TestFinalizeError)
{
    const std::string input("HolaEstoEsUnTest");

    // Crear un mock de OpenSSLAdapter
    auto mockAdapter = std::make_shared<MockOpenSSLAdapter>();

    // Establecer expectativas para simular un error durante la finalización
    EXPECT_CALL(*mockAdapter, initialize(HashAlgorithm::SHA256)).Times(1);
    EXPECT_CALL(*mockAdapter, update(input)).Times(1);
    EXPECT_CALL(*mockAdapter, finalize()).WillOnce(testing::Throw(std::runtime_error("Finalize failed")));

    // Pasar el adaptador a Hash256
    Hash256 hasher(mockAdapter);

    // Verificar que la excepción sea lanzada
    EXPECT_THROW(hasher.calculateHex(input), std::runtime_error);
}

// Comprobación de manejo de errores en la validación de hash.
TEST(Hash256, TestValidateHashError)
{
    const std::string input("HolaEstoEsUnTest");
    const std::string expected("2dc61c4df1a79838f548208438303ff07d154d7d2f9464c8cf308c59b6b49752");

    // Crear un mock de OpenSSLAdapter
    auto mockAdapter = std::make_shared<MockOpenSSLAdapter>();

    // Establecer expectativas para simular un error en la actualización
    EXPECT_CALL(*mockAdapter, initialize(HashAlgorithm::SHA256)).Times(1);
    EXPECT_CALL(*mockAdapter, update(input)).Times(1);
    EXPECT_CALL(*mockAdapter, finalize()).Times(1).WillOnce(testing::Throw(std::runtime_error("Finalize failed")));

    // Pasar el adaptador a Hash256
    Hash256 hasher(mockAdapter);

    // Verificar que la excepción sea lanzada cuando intentemos validar un hash
    EXPECT_THROW(hasher.verifyHexHash(input, expected), std::runtime_error);
}