#include <cstring>
#include <iostream>
#include "Generator.h"

int main(int argc, char **argv)
{
    bool encryption = true;
    if (std::strcmp(argv[1], "-e") == 0)
    {
        encryption = true;
    } else if (std::strcmp(argv[1], "-d") == 0)
    {
        encryption = false;
    } else
    {
        std::cout << "Please, use flag -e/-d to encode/decode file";
        return 1;
    }

    std::string str = argv[2];
    std::vector<uint8_t> k(str.length());
    for (int i = 0; i < str.length(); ++i)
    {
        k[i] = static_cast<unsigned char>(str[i]);
    }

    Generator generator(k);
    encryption ? generator.encryptFile(argv[3]) : generator.decryptFile(argv[3]);

    return 0;
}