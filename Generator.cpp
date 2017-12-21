
#include <fstream>
#include "Generator.h"
#include "Tester.h"

#define TESTING true

void readFile(std::vector<uint8_t> &msg, const std::string &path)
{
    FILE *f = fopen(path.c_str(), "rb");
    int c;

    while ((c = getc(f)) != EOF)
    {
        msg.push_back((uint8_t) c);
    }
    fclose(f);
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

bool Generator::LFSR1()
{
    dataOne = ((((dataOne >> 31) ^ (dataOne >> 30) ^ (dataOne >> 29)
                 ^ (dataOne >> 27) ^ (dataOne >> 25) ^ dataOne ) & 0x00000001 ) << 31 ) | (dataOne >> 1);
    return dataOne & 0x1;
}

bool Generator::LFSR2()
{
    dataTwo = ((((dataTwo >> 31) ^ (dataTwo >> 30) ^ (dataTwo >> 29)
                 ^ (dataTwo >> 27) ^ (dataTwo >> 25) ^ dataTwo ) & 0x00000001 ) << 31 ) | (dataTwo >> 1);
    return dataTwo & 0x1;
}

bool Generator::doTact()
{
    bool a1 = bit(dataOne, 32);
    bool a2 = bit(dataOne, 31);

    bool b1 = bit(dataTwo, 32);
    bool b2 = bit(dataTwo, 31);

    bool res = 0;

    if (a1 == 0 && a2 == 1)
    {
        res = LFSR1();
    } else if (b1 == 0 && b2 == 1)
    {
        res = LFSR2();
    } else
    {
        res = LFSR1();
        res ^= LFSR2();
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
            if (doTact())
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

    for (int i = 0; i < key.size() / 2; i++)
    {
        dataTwo += key[i];
    }

    for (auto i = key.size() / 2; i < key.size(); i++)
    {
        dataOne += key[i];
    }

}
