#include <cstdio>
#include <iostream>
#include <cmath>
#include <map>
#include "Tester.h"

void Tester::testAll()
{
    printf("\nData size (N): %lu\n\n", data.size());
    int onesIn = analyzeBits(data);
    std::cout << "Ones: " << onesIn << " Zeroes: " << (data.size() * 8) - onesIn << std::endl << std::endl;

    printf("Frequency Test: %f\n\n", freqTest(data));

    printf("Sequence Test: %f\n\n", seqTest(data));

    printf("Series Test: %f\n", seriesTest(data));

    autoTest(data);

    printf("Universal Test: %f\n\n", universalTest(data));

}

int Tester::countOnes(uint8_t byte)
{
    int counter = 0;

    for (int i = 0; i < 8; i++)
    {
        if (bit(byte, i) == 1)
        {
            counter++;
        }
    }
    return counter;
}

int Tester::analyzeBits(const std::vector<uint8_t> &message)
{
    int totalNumberOfOnes = 0;

    for (unsigned char i : message)
    {
        totalNumberOfOnes += countOnes(i);
    }

    return totalNumberOfOnes;
}

double Tester::freqTest(uint32_t ones, uint32_t N)
{
    return (2.0 / sqrt(N)) * (ones - (double) (N * 8 / 2));
}

double Tester::freqTest(const std::vector<uint8_t> &data)
{
    uint32_t N = data.size();
    uint32_t ones = analyzeBits(data);
    return freqTest(ones, N);
}

double Tester::seqTest(const std::vector<uint8_t> &data)
{
    std::vector<int> freq(256);
    int L = 8;
    unsigned long N = data.size();
    for (auto byte : data)
    {
        freq[byte] += 1;
    }

    double result = 0;

    for (int i : freq)
    {
        result += std::pow(i - (N * 8 / (L * 256.)), 2);
    }

    result = (result * L * 256.) / (N * 8);
    return result;
}

double Tester::seriesTest(std::vector<uint8_t> bits)
{
    std::map<int, int> zeros;
    std::map<int, int> ones;
    uint64_t N = bits.size();
    bool currBit;
    currBit = bit(bits[0], 0);
    std::string str;
    int len = 0;
    for (auto i : bits)
    {
        uint8_t byte = i;
        for (int j = 0; j < 8; j++)
        {
            if (bit(byte, j) == currBit)
            {
                len += 1;
            } else
            {
                if (len != 0)
                {
                    currBit == 1 ? ones[len]++ : zeros[len]++;
                }
                currBit = bit(byte, j);
                len = 0;
            }
        }
    }

    double ret = 0;
    int curr;
    for (int i = 1; i <= 15; ++i)
    {
        ones.find(i) == ones.end() ? curr = 0 : curr = ones[i];

        ret += (pow(curr - (N) / pow(2, i + 2), 2) / (N * 8 * (pow(2, i + 2))));
    }
    for (int i = 1; i <= 15; ++i)
    {
        zeros.find(i) == zeros.end() ? curr = 0 : curr = zeros[i];

        ret += (pow(curr - (N) / pow(2, i + 2), 2) / (N * 8 * (pow(2, i + 2))));
    }
    return ret;
}

void Tester::autoTest(std::vector<unsigned char> bits)
{
    std::cout <<  "\n======AUTOCORRELATION======\n";
    for (int tau = 10; tau < 30; tau += 5)
    {
        int ones = 0;
        for (int i = 0; i < bits.size() - tau; ++i)
        {
            for (int j = 0; j < 8; j++)
            {
                bit(bits[i], j) ^ bit(bits[i], j + tau) == 1 ? ones++ : 0;
            }
        }
        std::cout << "Tau = " << tau << " freqTest = " << freqTest(ones, bits.size() - tau) << std::endl;
    }
    std::cout << std::endl;
}

double Tester::universalTest(std::vector<uint8_t> bits)
{
    std::vector<int> s;
    std::vector<int> tabs(256);

    int l = 8;
    auto Q = static_cast<uint32_t>(pow(2, l) * 10);
    double K;

    for (int i = 0; i < bits.size() - l; i += l)
    {
        for (int j = 0; j < 8; j++)
        {
            s.push_back(bit(bits[i], j) ? 1 : 0);
        }
    }

    K = s.size() - Q;

    for (int i = 0; i < Q; ++i)
    {
        tabs[s[i]] = i;
    }

    double sum = 0;
    for (uint32_t i = Q; i < Q + K; ++i)
    {
        sum += (log2(i - tabs[s[i]]));
    }

    double e = 7.1836656;
    double d = 3.238;
    double c = 0.7 - 0.8 / l + ((4 + 32 / l) * pow(K, -(double) 3 / l)) / 15;
    sum = sum / K;
    double z = (sum - e) / (c * sqrt(d));
    return z;
}
