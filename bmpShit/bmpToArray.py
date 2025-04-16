from PIL import Image
import numpy as np

FILENAME = "IMG_0212.bmp"

def convertRGB(r, g, b):
  return (((b & 0xF8) >> 3) << 11) | (((g & 0xFC) >> 2) << 5) | (r >> 3)


img = Image.open(FILENAME).convert("RGB")
width, height = img.size
pixels = np.array(img)

# Prepare output array
rgb565_array = np.zeros((height, width), dtype=np.uint16)

print(height)
print(width)

pixels = np.flipud(pixels)  # Flip vertically

for y in range(height):
  for x in range(width):
    r, g, b = pixels[y, x]
    rgb565_array[y, x] = convertRGB(int(r), int(g), int(b))

with open("output.txt", "w") as f:
  print("{", end='', file=f)
  for i in rgb565_array:
    for j in i:
      if j == 59294:
        print (str(0xF81F)+',', end='', file=f)
      else:
        print (str(j)+',', end='', file=f)
    
    # f.seek(f.tell() - 1)
    # print("},", end='', file=f)
  
  f.seek(f.tell() - 1)
  print("}", end='', file=f)