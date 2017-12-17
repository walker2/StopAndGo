#ifndef TWO_WAY_GENERATOR_H
#define TWO_WAY_GENERATOR_H


#include <cstdint>
#include <utility>
#include <vector>
#include <string>

static const uint64_t firstConst[2] = {
        0x976a2d15490d6693,
        0x844dc6ff26ad7a9a,
};

static const uint64_t secondConst[2] = {
        0x844dc6ff26ad7a9a,
        0x976a2d15490d6693,
};

class Generator
{
public:
    Generator() = default;

    explicit Generator(std::vector<uint8_t> _key) : key(std::move(_key)) { applyKey(); }

    void encryptFile(const std::string &path);

    void decryptFile(const std::string &path);

private:

    void cipherFile(const std::string &path, bool isEncryption);

    bool doLFSR(uint64_t *p);

    bool doTact(uint64_t *first, uint64_t *second);

    void applyKey();

private:
    std::vector<uint8_t> key;
    uint64_t registerOneData[2];
    uint64_t registerTwoData[2];
};


#endif
