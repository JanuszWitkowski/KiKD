from PIL import Image
import numpy as np
import sys

filename = sys.argv[1]
newname = sys.argv[2]
with open(filename) as f:
    numbers = f.read().splitlines()

w = int(numbers[0])
h = int(numbers[1])
data = np.zeros((h, w, 3), dtype=np.uint8)
i, width, height = 2, 0, 0
while i < len(numbers):
    # print(i)
    blue = np.uint8(numbers[i])
    green = np.uint8(numbers[i+1])
    red = np.uint8(numbers[i+2])
    data[height][width] = [red, green, blue]
    width = (width+1)%w
    if width == 0:
        height = (height+1)%h
    i += 3

img = Image.fromarray(data, 'RGB')
img.save(newname)
# img.show()
