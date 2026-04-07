#ifndef QUANTSTREAM_H
#define QUANTSTREAM_H

#include <cstdint>

class QuantStream {
public:
    static void pack_32_10bit(const uint16_t* in, uint32_t* out);
    static void run_benchmark(const char* filepath);
};

#endif