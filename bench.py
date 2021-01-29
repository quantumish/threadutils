import subprocess
import time
import matplotlib.pyplot as plt

def bench(n):
    before = time.time()
    subprocess.check_output("./hash " + str(n), shell=True)
    after = time.time()
    return after-before

times=[]
i = 10**6
while (i < (10**6 * 100)):
    print(i/(10**6))
    times.append(bench(i))
    i+=10**6

plt.plot(times)
plt.show()
