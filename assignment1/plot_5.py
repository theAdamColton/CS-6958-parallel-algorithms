import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import os
from io import StringIO

d_strong = []

d = []

for n_threads in (1, 2, 4, 8, 12, 16, 24, 32):
    os.environ["OMP_NUM_THREADS"] = str(n_threads)

    task = subprocess.run(["./task5"], stdout=subprocess.PIPE)

    d_run = pd.read_csv(StringIO(task.stdout.decode()))
    d_run['n_threads'] = n_threads


    d.append(d_run)

d = pd.concat(d)

d['speedup'] = d.time_seq / d.time_par

m = d.m.max()
n = d.n.max()
d_strong =d[(d.m == m) & (d.n == n)]


d_strong.boxplot('speedup', 'n_threads')
plt.title("strong scaling")
plt.xlabel("n threads (16 threads total on machine)")
plt.ylabel("speedup")
plt.show()

d_weak = d[d.n_threads == 32]
d_weak.boxplot('speedup', 'n')
plt.title("weak scaling (n threads kept at 16)")
plt.xlabel("n (matrix rows)")
plt.ylabel("speedup")
plt.show()
