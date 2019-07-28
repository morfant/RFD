import serial

#port = "/dev/ttyUSB0"
#port = "/dev/ttyACM0"
port = "/dev/ttyACM1"

sr = serial.Serial(port, 230400)
sr.flushInput()

while True:
    x = sr.readline()
    print(x)
