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

    bool doTact(uint32_t first, uint32_t second);

    void applyKey();

private:
    std::vector<uint8_t> key;
    uint32_t dataOne = 0x976a2d15;
    uint32_t dataTwo = 0x844dc6ff;

    bool LFSR1();

    bool LFSR2();
};


#endif
