import subprocess
import os
import time
import matplotlib.pyplot as plt

times = []
n_threads_to_try = range(8, 8*4+1)
n_trials = 16

for n_threads in n_threads_to_try:
    os.environ["OMP_NUM_THREADS"] = str(n_threads)

    ttime = 0.

    for _ in range(n_trials):
        st = time.time()
        subprocess.run(["./hello"],
            stdout=subprocess.DEVNULL,)
        et = time.time()
        ttime += et - st

    times.append(ttime / n_trials)

plt.plot(n_threads_to_try, times)
plt.xlabel("n threads")
plt.ylabel("time (s.)")
plt.title(f"time to initialize 1m random long ints vs. number of threads (avg over {n_trials})")
plt.show()

