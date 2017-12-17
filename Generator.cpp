
#include <fstream>
#include "Generator.h"
#include "Tester.h"

#define TAP_MASK_LOW64 0x0
#define TAP_MASK_HIGH64 0x28000005
#define TESTING true

void readFile(std::vector<uint8_t> &msg, const std::string &path)
{
    FILE *f = fopen(path.c_str(), "rb");
    int c;

    while ((c = getc(f)) != EOF)
    {
        msg.push_back((uint8_t) c);
    }
}

void writeFile(const std::vector<uint8_t> &msg, const std::string &path)
{
    std::ofstream out(path);

    for (auto b : msg)
    {
        out << b;
    }

    out.close();
}

bool Generator::doLFSR(uint64_t *p)
{
    uint64_t low = p[0] & TAP_MASK_LOW64;
    uint64_t high = p[1] & TAP_MASK_HIGH64;

    uint64_t s1 = low ^high;
    uint32_t s2 = (uint32_t) s1 ^(uint32_t) (s1 >> 32);
    uint32_t s3 = (s2 & 0xffff) ^(s2 >> 16);
    uint32_t s4 = (s3 & 0xff) ^(s3 >> 8);
    uint32_t s5 = (s4 & 0xf) ^(s4 >> 4);
    uint32_t s6 = (s5 & 0x3) ^(s5 >> 2);
    uint32_t s7 = (s6 & 0x1) ^(s6 >> 1);
    p[1] = (p[1] << 1) | (p[0] >> 63);
    p[0] = (p[0] << 1) | s7;
    return static_cast<bool>(s7);
}

bool Generator::doTact(uint64_t *first, uint64_t *second)
{
    bool a1 = bit(first[0], 0);
    bool a2 = bit(first[0], 1);

    bool b1 = bit(second[0], 0);
    bool b2 = bit(second[0], 1);

    bool res = 0;

    if (a1 == 0 && a2 == 1)
    {
        res = doLFSR(first);
    } else if (b1 == 0 && b2 == 1)
    {
        res = doLFSR(second);
    } else
    {
        res = doLFSR(first);
        res ^= doLFSR(second);
    }
    return res;
}

void Generator::encryptFile(const std::string &path)
{
    cipherFile(path, true);
}

void Generator::decryptFile(const std::string &path)
{
    cipherFile(path, false);
}

void Generator::cipherFile(const std::string &path, bool isEncryption)
{
    std::vector<uint8_t> message;
    std::vector<uint8_t> result;
    std::vector<uint8_t> generatedData;

    readFile(message, path);
    volatile unsigned i;

    for (i = 0; i < message.size(); ++i)
    {
        uint8_t byte = message[i];
        uint8_t generatedByte = 0x0;
        for (int j = 0; j < 8; ++j)
        {
            if (doTact(registerOneData, registerTwoData))
                generatedByte |= 1UL << j;
        }
        generatedData.push_back(generatedByte);

        byte ^= generatedByte;
        result.push_back(byte);
    }

#ifdef TESTING
    Tester tester(generatedData);
    tester.testAll();
#endif

    std::string addMsg = isEncryption ? "_encrypted" : "_decrypted";
    size_t lastdot = path.find_last_of('.');

    if (lastdot != std::string::npos)
    {
        std::string beforeDot = path.substr(0, lastdot);
        std::string afterDot = path.substr(lastdot, path.size() - 1);
        writeFile(result, beforeDot + addMsg + afterDot);
    } else
    {
        writeFile(result, path + addMsg);
    }
}

void Generator::applyKey()
{
    registerOneData[0] = firstConst[0];
    registerOneData[1] = firstConst[1];

    registerTwoData[0] = secondConst[0];
    registerTwoData[1] = secondConst[1];

    for (int i = 0; i < key.size() / 2; i++)
    {
        registerOneData[0] += key[i];
        registerTwoData[1] += key[i];
    }

    for (auto i = key.size() / 2; i < key.size(); i++)
    {
        registerOneData[1] += key[i];
        registerTwoData[0] += key[i];
    }

#ifdef DEBUG
    printf("%016"PRIx64":%016"PRIx64"\n", registerOneData[1], registerOneData[0]);
    printf("%016"PRIx64":%016"PRIx64"\n", registerTwoData[1], registerTwoData[0]);
#endif

}
