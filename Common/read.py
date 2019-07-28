import serial
import sys

#port = "/dev/ttyUSB0"
#port = "/dev/ttyACM0"
port = sys.argv[1]

sr = serial.Serial(port, 230400)
sr.flushInput()

while True:
    x = sr.readline()
    print(x)
