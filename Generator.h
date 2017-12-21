#ifndef TWO_WAY_GENERATOR_H
#define TWO_WAY_GENERATOR_H


#include <cstdint>
#include <utility>
#include <vector>
#include <string>

class Generator
{
public:
    Generator() = default;

    explicit Generator(std::vector<uint8_t> _key) : key(std::move(_key)) { applyKey(); }

    void encryptFile(const std::string &path);

    void decryptFile(const std::string &path);

private:

    void cipherFile(const std::string &path, bool isEncryption);

    bool doTact();

    void applyKey();

private:
    std::vector<uint8_t> key;
    uint32_t dataOne = 0xfffffff;
    uint32_t dataTwo = 0xfffffff;

    bool LFSR1();

    bool LFSR2();
};


#endif
