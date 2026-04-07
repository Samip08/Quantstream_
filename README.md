# QuantStream

High-performance 10-bit integer packing engine using AVX2 SIMD and OpenMP.

## Requirements

- **OS**: Linux (Ubuntu/Debian recommended)
- **Compiler**: g++ (GCC) 9.0+
- **Hardware**: CPU with AVX2 and BMI2 support
- **Libraries**: OpenMP (`libomp-dev`)

## Performance Snapshot

| Mode | Throughput |
|------|-----------|
| Baseline | ~2.7 GB/s |
| QuantStream | ~9.4 GB/s |
| Speedup | ~3.4x |

## Setup & Build

### 1. Fix Makefile Formatting

If you are copy-pasting the Makefile and getting `missing separator` errors, run the following command to regenerate it with correct tab indentation:

```bash
cat << 'EOF' > Makefile
CXX = g++
CXXFLAGS = -O3 -mavx2 -mbmi2 -fopenmp -Iinclude
SRC = src/quantstream.cpp tests/main.cpp
TARGET = quantstream_bench

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
EOF
```

### 2. Generate Data

```bash
python3 data/generate_data.py
```

### 3. Compile and Run

```bash
make clean && make
./quantstream_bench
```
