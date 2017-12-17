#ifndef TWO_WAY_TESTER_H
#define TWO_WAY_TESTER_H

#define bit(x, n)   ((1 << n) & x) != 0
#include <cstdint>
#include <utility>
#include <vector>

class Tester
{
public:
    Tester() = default;

    explicit Tester(std::vector<uint8_t> _data) : data(std::move(_data)) {}

    void testAll();

private:
    std::vector<uint8_t> data;

    int countOnes(uint8_t byte);

    int analyzeBits(const std::vector<uint8_t> &message);

    double freqTest(uint32_t ones, uint32_t N);

    double freqTest(const std::vector<uint8_t> &data);

    double seqTest(const std::vector<uint8_t> &data);

    double seriesTest(std::vector<uint8_t> bits);

    void autoTest(std::vector<unsigned char> bits);

    double universalTest(std::vector<uint8_t> bits);
};


#endif
