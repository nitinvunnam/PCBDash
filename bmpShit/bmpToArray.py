# Aditya Pulipaka
# BMP to bitmap array converter.
# Change FILENAME to your bmp file's name. BMP file should be in same directory as this python file.
# pip install pillow and numpy
# The IGNORE_X colors will likely not be accurate at first, just run it once on your file, then check output.txt
# The number you see the most like at the beginning and the end will be the color at the bottom left and top right corners
# which will most likely be your background. You can replace the IGNOREs with these number(s). If you don't need all of them, set them to the same number(s).
# Height and width of bitmap will be output to console in that order. Multiply these to get your array size.
# output.txt will hold the array.

from PIL import Image
import numpy as np

FILENAME = "bmpShit/button.bmp"
IGNORE_0 = 65535
IGNORE_1 = 65535
IGNORE_2 = 65535
IGNORE_3 = 65535

def convertRGB(r, g, b):
  return (((b & 0xF8) >> 3) << 11) | (((g & 0xFC) >> 2) << 5) | (r >> 3)


img = Image.open(FILENAME).convert("RGB")
width, height = img.size
pixels = np.array(img)
pixels = np.flipud(pixels)  # Flip vertically

# upperBound = height
# lowerBound = 0
# leftBound = 0
# rightBound = width

# x = 0
# nonIgnoreFound = False
# while not nonIgnoreFound: # find leftBound
#   for i in range(height):
#     r, g, b = pixels[i, x]
#     value = convertRGB(int(r), int(g), int(b))
#     if not (value == IGNORE_0 or value == IGNORE_1):
#       nonIgnoreFound = True
#       leftBound = x

#   x+=1
  
# x = width - 1
# nonIgnoreFound = False
# while not nonIgnoreFound: # find rightBound
#   for i in range(height):
#     r, g, b = pixels[i, x]
#     value = convertRGB(int(r), int(g), int(b))
#     if not (value == IGNORE_0 or value == IGNORE_1):
#       nonIgnoreFound = True
#       rightBound = x

#   x-=1

# y = 0
# nonIgnoreFound = False
# while not nonIgnoreFound: # find lowerBound
#   for i in range(width):
#     r, g, b = pixels[y, i]
#     value = convertRGB(int(r), int(g), int(b))
#     if not (value == IGNORE_0 or value == IGNORE_1):
#       nonIgnoreFound = True
#       lowerBound = y

#   y+=1

# y = height - 1
# nonIgnoreFound = False
# while not nonIgnoreFound: # find upperBound
#   for i in range(width):
#     r, g, b = pixels[y, i]
#     value = convertRGB(int(r), int(g), int(b))
#     if not (value == IGNORE_0 or value == IGNORE_1):
#       nonIgnoreFound = True
#       upperBound = y

#   y-=1

# Prepare output array
rgb565_array = np.zeros((height, width), dtype=np.uint16)

print(height)
print(width)

for y in range(height):
  for x in range(width):
    r, g, b = pixels[y, x]
    rgb565_array[y, x] = convertRGB(int(r), int(g), int(b))

with open("output.txt", "w") as f:
  print("{", end='', file=f)
  for i in rgb565_array:
    for j in i:
      if j == IGNORE_0 or j == IGNORE_1 or j == IGNORE_2 or j == IGNORE_3:
        print (str(0xF81F)+',', end='', file=f)
      else:
        print (str(j)+',', end='', file=f)
  
  f.seek(f.tell() - 1)
  print("}", end='', file=f)