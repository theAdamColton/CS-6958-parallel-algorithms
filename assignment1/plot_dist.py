import sys
import matplotlib.pyplot as plt

arr = []
for line in sys.stdin:
    line = line.strip()
    if line != "":
        arr.append(int(line))

plt.hist(arr, 200)
plt.title(f"distribution of {len(arr)} longs")
plt.show()

