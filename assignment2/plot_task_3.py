import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import os
from io import StringIO

d = []
for n_threads in (1, 2, 4, 8, 16,):
    os.environ["OMP_NUM_THREADS"] = str(n_threads)

    task = subprocess.run(["./task3"], stdout=subprocess.PIPE)

    d_run = pd.read_csv(StringIO(task.stdout.decode()))
    d_run['n_threads'] = n_threads


    d.append(d_run)

d = pd.concat(d)

max_n = d.n.max()

d_strong = d[d.n == max_n].groupby('n_threads').mean()
d_strong.plot(y='time')
plt.title(f"strong scaling, n held at {max_n}")
plt.xlabel('n_threads')
plt.ylabel("time (ms)")
plt.show()

d['n_per_thread'] = d.n // d.n_threads
n_per_thread = d.n_per_thread.value_counts().index[0]
d_weak = d[d['n_per_thread'] == n_per_thread].groupby('n').mean()
#d.groupby(by=['n','n_threads']).mean()
d_weak.plot(x="n_threads", y="time")
plt.title(f"weak scaling, n_per_thread kept at {n_per_thread}")
plt.ylabel("time (ms)")
plt.show()
