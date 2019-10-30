from datetime import datetime
import os
import time

path = "C:\\Users\\LattePanda\\RFD\\Rover\\log"
ext = '.log'

now = datetime.now()
y = now.year
m = now.month
d = now.day
h = now.hour
M = now.minute
s = now.second


date = str(y) + "_" + str(m) + "_" + str(d) + "_" + str(h) + ":" + str(M) + ":" + str(s)


filename = "extIp_" + date + ext
# print(filename)
cmd_0 = "echo >> test.txt"
cmd_1 = 'curl -s icanhazip.com >> test.txt'
cmd_2 = 'date +"%T" >> test.txt'
cmd_3 = 'echo "-----\n" >> test.txt'

while True:
    os.system(cmd_0)
    os.system(cmd_1)
    # os.system(cmd_2)
    # os.system(cmd_3)
    time.sleep(10)
