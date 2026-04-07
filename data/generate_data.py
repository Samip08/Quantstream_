import numpy as np
import os

os.makedirs('data', exist_ok=True)

N = int(5e8)  # 500 million uint16 = 1GB
print("Generating 1GB of 10-bit sensor data...")
data = np.random.randint(0, 1024, size=N, dtype=np.uint16)
data.tofile("data/sensor_data.bin")
print("Done: data/sensor_data.bin")