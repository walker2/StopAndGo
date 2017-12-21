#include <cstdio>
#include <iostream>
#include <cmath>
#include <map>
#include "Tester.h"

void Tester::testAll()
{
    printf("\nData size (N): %lu bits\n\n", data.size() * 8);
    int onesIn = analyzeBits(data);
    std::cout << "Ones: " << onesIn << " Zeroes: " << (data.size() * 8) - onesIn << std::endl << std::endl;

    double freqTestResult = freqTest(data);
    printf("Frequency Test: %f", freqTestResult);
    if (freqTestResult >= -3 && freqTestResult <= 3)
        printf(" => passed \n\n");
    else
        printf(" => not passed \n\n");

    double seqTestResult = seqTest(data);
    printf("Sequence Test: %f", seqTestResult);
    if (seqTestResult <= 284.3359)
        printf(" => passed\n\n");
    else
        printf(" => not passed\n\n");

    double seriesTestResult = seriesTest(data);
    printf("Series Test: %f", seriesTestResult);
    if (seriesTestResult <= 40.2560)
        printf(" => passed\n");
    else
        printf(" => not passed\n");

    autoTest(data);

    double universalTestResult = universalTest(data);
    printf("Universal Test: %f", universalTestResult);

    if (universalTestResult >= -1.96 && universalTestResult <= 1.96)
        printf(" => passed\n");
    else
        printf(" => not passed\n");

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
    unsigned long N = data.size() * 8;
    for (auto byte : data)
    {
        freq[byte] += 1;
    }

    double result = 0;

    for (int i : freq)
    {
        result += std::pow(i - (N / (L * 256.)), 2);
    }

    result = (result * L * 256.) / (N);
    return result;
}

double Tester::seriesTest(std::vector<uint8_t> data)
{
    std::vector<int> zeros(16);
    std::vector<int> ones(16);

    uint64_t N = data.size() * 8;
    bool currBit;
    currBit = bit(data[0], 0);
    std::string str;
    int len = 0;
    for (auto byte : data)
    {
        for (int j = 0; j < 8; j++)
        {
            if (bit(byte, j) == currBit)
            {
                len += 1;
            } else
            {
                if (len != 0)
                {
                    currBit == 1 ? ones[len - 1]++ : zeros[len - 1]++;
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
        curr = ones[i];
        ret += (pow(curr - (N / pow(2, i + 2)), 2) / (N / (pow(2, i + 2))));
    }
    for (int i = 1; i <= 15; ++i)
    {
        curr = zeros[i];
        ret += (pow(curr - (N / pow(2, i + 2)), 2) / (N / (pow(2, i + 2))));
    }
    return ret;
}

void Tester::autoTest(std::vector<unsigned char> bits)
{
    std::cout << "\n======AUTOCORRELATION======\n";
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
        double res = freqTest(ones, bits.size() - tau);
        std::cout << "Tau = " << tau << " freqTest = " << res;
        if (res >= -3 && res <= 3)
            printf(" => passed \n");
        else
            printf(" => not passed \n");
    }
    std::cout << std::endl;
}

double Tester::universalTest(std::vector<uint8_t> data)
{
    std::vector<int> s;
    std::vector<int> tabs(256);

    int l = 8;
    auto Q = static_cast<uint32_t>(pow(2, l) * 10);
    double K;

    K = data.size() - Q;

    for (int i = 0; i < Q; ++i)
    {
        tabs[data[i]] = i;
    }

    double sum = 0;
    for (uint32_t i = Q; i < Q + K; ++i)
    {
        sum += (log2(i - tabs[data[i]]));
    }

    double e = 7.1836656;
    double d = 3.238;
    double c = 0.7 - 0.8 / l + ((4 + 32 / l) * pow(K, -(double) 3 / l)) / 15;
    sum = sum / K;
    double z = (sum - e) / (c * sqrt(d));

    return z;
}
