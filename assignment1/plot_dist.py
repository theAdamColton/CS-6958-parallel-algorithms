import sys
import matplotlib.pyplot as plt

arr = []
for line in sys.stdin.readlines():
    line = line.strip()
    if line != "":
        arr.append(int(line))


print("got arr")

plt.hist(arr, 200)
plt.show()
