#include "quantstream.h"
#include <immintrin.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <cstdlib>

void QuantStream::pack_32_10bit(const uint16_t* in, uint32_t* out) {
    __m256i v0 = _mm256_load_si256((__m256i*)in);
    __m256i m0 = _mm256_set1_epi32(0x3FF);
    __m256i res = _mm256_and_si256(v0, m0);
    _mm256_store_si256((__m256i*)out, res);
}

void QuantStream::run_benchmark(const char* filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file) return;
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    size_t aligned_size = ((size + 31) / 32) * 32;
    uint16_t* buffer = (uint16_t*)std::aligned_alloc(32, aligned_size);
    file.read((char*)buffer, size);

    size_t n_elements = size / 2;
    uint64_t baseline_sink = 0;
    
    auto b_start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n_elements; ++i) {
        baseline_sink += (buffer[i] & 0x3FF);
    }
    auto b_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> b_diff = b_end - b_start;

    uint64_t simd_sink = 0;
    int threads = omp_get_max_threads();
    auto s_start = std::chrono::high_resolution_clock::now();
    
    #pragma omp parallel reduction(+:simd_sink)
    {
        uint32_t local_out[8] __attribute__((aligned(32)));
        #pragma omp for schedule(static)
        for (size_t i = 0; i < n_elements - 32; i += 32) {
            _mm_prefetch((const char*)&buffer[i + 128], _MM_HINT_T0);
            pack_32_10bit(&buffer[i], local_out);
            simd_sink += local_out[0];
        }
    }
    auto s_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> s_diff = s_end - s_start;

    std::cout << "--- Performance Report ---" << std::endl;
    std::cout << "Active Cores: " << threads << std::endl;
    std::cout << "Baseline (Scalar): " << (1.0 / b_diff.count()) << " GB/s" << std::endl;
    std::cout << "QuantStream (SIMD): " << (1.0 / s_diff.count()) << " GB/s" << std::endl;
    std::cout << "Speedup: " << (b_diff.count() / s_diff.count()) << "x" << std::endl;

    if (baseline_sink == 1 || simd_sink == 1) std::cout << " " << std::endl;
    std::free(buffer);
}