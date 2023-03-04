import matplotlib.pyplot as plt
import sys
import time

data = input().split() # get data from cpp file

n = list(map(int, data[::3]))
memory_size_cpp = list(map(int, data[1::3]))
time_cpp = list(map(float, data[2::3]))

memory_size_py = []
time_py = []
testdict = {}
for i in range(6):
    start = time.perf_counter()
    testdict.clear()
    num = 10 ** i
    for j in range(num):
        testdict[j] = j
    finish = time.perf_counter()
    duration = finish - start
    memory_size_py.append(sys.getsizeof(testdict) / (1024 ** 2))
    time_py.append(duration * 1000 ** 3)    

plt.figure()
plt.subplot(121)
plt.plot(n, memory_size_cpp, label="cpp memory")
plt.plot(n[:6], memory_size_py, label="python memory")
plt.xlabel("elements")
plt.ylabel("MB")
plt.legend()
plt.subplot(122)
plt.plot(n, time_cpp, label="cpp time")
plt.plot(n[:6], time_py, label="python time")
plt.xlabel("elements")
plt.ylabel("ns")
plt.legend()
plt.show()
