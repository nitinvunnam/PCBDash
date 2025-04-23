import numpy as np
from scipy.io import wavfile

rate, data = wavfile.read("wavShit/tripped.wav")  # 8-bit unsigned PCM

# Confirm dtype is uint8 and range is 0–255
assert data.dtype == np.uint8

# Shift to 5-bit
data_5bit = (data >> 3).astype(np.uint8)  # 0–31 range
print(rate)
print (len(data_5bit))

with open("output.txt", "w") as f:
  print("{", end='', file=f)
  for i in data_5bit:
    print (str(i)+',', end='', file=f)

  f.seek(f.tell() - 1)
  print("}", end='', file=f)