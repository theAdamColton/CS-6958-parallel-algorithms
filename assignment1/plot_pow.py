import subprocess
import os
import time

tt = 0.
trials = 10
os.environ["OMP_NUM_THREADS"] = '32'

for _ in range(trials):
    st = time.time()
    subprocess.run(['./hello'])
    et = time.time()
    tt += et - st

print(tt / trials, 'seconds')
