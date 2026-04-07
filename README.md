# QuantStream 

High-performance 10-bit integer packing engine using AVX2 SIMD and OpenMP.

## Requirements
- **OS**: Linux (Ubuntu/Debian recommended)
- **Compiler**: g++ (GCC) 9.0+
- **Hardware**: CPU with AVX2 and BMI2 support
- **Libraries**: OpenMP (`libomp-dev`)

## Performance Snapshot
Baseline: ~2.7 GB/s
QuantStream: ~9.4 GB/s
Real-world Speedup: ~3.4x

## Setup & Build

### 1. Fix Makefile Formatting
If you are copy-pasting code and getting "missing separator" errors, run this command to generate the Makefile with correct tab indentation:

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

### 2.Generate data locally
python3 data/generate_data.py

#### 3. compile and run
make clean && make
./quantstream_bench