from datetime import datetime
import os
import time

path = './log/'
ext = '.log'

now = datetime.now()
y = now.year
m = now.month
d = now.day
h = now.hour
M = now.minute
s = now.second


date = str(y) + "_" + str(m) + "_" + str(d) + "_" + str(h) + ":" + str(M) + ":" + str(s)

filename =  path + "extIp_" + date + ext

cmd_0 = 'echo "-----" >> ' + filename
cmd_1 = 'curl -s icanhazip.com >> ' + filename
cmd_2 = 'date +"%T" >> ' + filename
cmd_3 = 'echo "-----\n" >> ' + filename

while True:
    os.system(cmd_0)
    os.system(cmd_1)
    os.system(cmd_2)
    os.system(cmd_3)
    time.sleep(10)
