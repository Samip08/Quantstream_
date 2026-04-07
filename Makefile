CXX = g++
CXXFLAGS = -O3 -mavx2 -mbmi2 -fopenmp -Iinclude
SRC = src/quantstream.cpp tests/main.cpp
TARGET = quantstream_bench

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
