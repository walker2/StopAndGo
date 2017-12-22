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

double Tester::seriesTest(const std::vector<uint8_t> &data)
{
    int L = 15;
    uint64_t N = data.size();
    std::vector<uint64_t> sequencesZero(L + 1);
    std::vector<uint64_t> sequenceOne(L + 1);

    int lenOne = 0;
    int lenZero = 0;

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            int bit = bit(data[i], j);
            if (bit == 1)
            {
                lenOne++;
                if (lenZero <= L)
                    sequencesZero[lenZero]++;
                lenZero = 0;
            } else
            {
                lenZero++;
                if (lenOne <= L)
                    sequenceOne[lenOne]++;
                lenOne = 0;
            }
        }
    }

    double ones = 0;
    double zeroes = 0;

    for(int i = 1; i < L + 1; i++)
    {
        double magnitude = ((N * 8) / pow(2, i + 2));
        ones += pow(sequenceOne[i] - magnitude, 2) / magnitude;
        zeroes += pow(sequencesZero[i] - magnitude, 2) / magnitude;
    }

    double sum = ones + zeroes;
    return  sum;
}

void Tester::autoTest(const std::vector<uint8_t> &data)
{
    std::cout << "\n======AUTOCORRELATION======\n";
    for (int tau = 10; tau < 30; tau += 5)
    {
        int ones = 0;
        for (int i = 0; i < data.size() - tau; ++i)
        {
            for (int j = 0; j < 8; j++)
            {
                bit(data[i], j) ^ bit(data[i], j + tau) == 1 ? ones++ : 0;
            }
        }
        double res = freqTest(ones, data.size() - tau);
        std::cout << "Tau = " << tau << " freqTest = " << res;
        if (res >= -3 && res <= 3)
            printf(" => passed \n");
        else
            printf(" => not passed \n");
    }
    std::cout << std::endl;
}

double Tester::universalTest(const std::vector<uint8_t> &data)
{
    int L = 8;
    int N = data.size();
    double V = pow(2,L);

    auto Q = static_cast<uint32_t>(pow(2, L) * 10);
    double K;
    K = data.size() - Q;

    std::vector<int> table(V);

    std::vector<int> parts;
    int bit = 0;
    int tmp = 0;
    int z = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            bit = bit(data[i], j);

            tmp += bit * pow(2,z);
            z++;
            if (z == L)
            {
                parts.push_back(tmp);
                z = 0;
                tmp = 0;
            }
        }
    }

    int i = 0;
    for (i; i < Q; i++)
    {
        int b = parts[i];
        table[b] = i;
    }

    double sum = 0;

    for (i; i < Q + K; i++)
    {
        int b = parts[i];
        sum += log2(i - table[b]);
        table[b] = i;
    }

    sum = sum / K ;

    double e = 7.1836656;
    double d = 3.238;
    double C = 0.7 - 0.8 / L + ((4 + 32 / L) * pow(K, -(double) 3 / L)) / 15;

    double Z = (sum - e) / (C * sqrt(d));
    return Z;
}
