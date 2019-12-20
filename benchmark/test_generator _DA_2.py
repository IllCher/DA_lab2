import string
import random
import sys

def id_generator(size):
    return ''.join(random.choice(string.ascii_lowercase) for _ in range(size))
f = open('cmake-build-debug/test.txt', 'w')
i = 0
while i < 1000000:
    f.write(id_generator(random.randint(1,256)) + ' ' + str(random.randint(0, 18446744073709551616)) + '\n')
    i = i + 1
f.close()
