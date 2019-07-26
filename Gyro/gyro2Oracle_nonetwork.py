########################################################################
#
# Copyright (c) 2017, STEREOLABS.
#
# All rights reserved.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
########################################################################

import serial
import time

import socket
import select
import sys

def main():
    if len(sys.argv) < 3:
        print("Use default setting. 132.145.113.171:9900")
        # HOST = "132.145.113.171"
        HOST = "121.162.9.196"
        PORT = 9901
    else:
        HOST = sys.argv[1]
        PORT = int(sys.argv[2])

    MASTER_SOCK = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    MASTER_SOCK.settimeout(200)

    # connect to remote host
    try:
        MASTER_SOCK.connect((HOST, PORT))
    except Exception as msg:
        print(type(msg).__name__)
        print("Unable to connect")
        sys.exit()

    print("Connected to remote host. Start sending messages")


    # serial
    ser = serial.Serial('/dev/cu.usbmodem14601', 38400) 
    time.sleep(1)
    # print(ser)

    m_values = [0, 0, 0]
    a_values = [0, 0, 0]
    g_values = [0, 0, 0]
    y_values = [0, 0, 0]

    while True:
   
        # Get sensor values from arduino
        b = ser.readline()
        print(b)
        string_b = b.decode()
        # print(string_b)
        string = string_b.rstrip()
        # print(string)

        v = string.split(' ')
        if len(v) == 13 and v[0] == 's':
            m_values[0] = float(v[1])
            m_values[1] = float(v[2])
            m_values[2] = float(v[3])
            a_values[0] = float(v[4])
            a_values[1] = float(v[5])
            a_values[2] = float(v[6])
            g_values[0] = float(v[7])
            g_values[1] = float(v[8])
            g_values[2] = float(v[9])
            y_values[0] = float(v[10])
            y_values[1] = float(v[11])
            y_values[2] = float(v[12])

            # print(m_values)
            # print(a_values)
            # print(g_values)
            # print(y_values)

            # Display the translation and timestamp
            # print("Time stamp: {0}\n".format(time_stamp))
            time_stamp = float(time.time())

            #Display mag values
            mx = (m_values[0])
            my = (m_values[1])
            mz = (m_values[2])
            # print("Mag x, y, z: {0}, {1}, {2}\n".format(mx, my, mz))
            mag = "{0},{1},{2}".format(mx, my, mz)

            #Display acc values
            ax = (a_values[0])
            ay = (a_values[1])
            az = (a_values[2])
            # print("Acc x, y, z: {0}, {1}, {2}\n".format(ax, ay, az))
            acc = "{0},{1},{2}".format(ax, ay, az)
            
            #Display gyro values
            gx = (g_values[0])
            gy = (g_values[1])
            gz = (g_values[2])
            # print("Gyro x, y, z: {0}, {1}, {2}\n".format(gx, gy, gz))
            gyro = "{0},{1},{2}".format(gx, gy, gz)

            # Display yaw, pitch, roll
            y = (y_values[0])
            p = (y_values[1])
            r = (y_values[2])
            # print("Yaw, Roll, Pitch: {0}, {1}, {2}\n".format(y, p, r))
            ypr = "{0},{1},{2}".format(y, p, r)

            msg = "{0},{1},{2},{3},{4}".format(time_stamp, mag, acc, gyro, ypr) # yrp = yaw, pitch, roll
    #        MASTER_SOCK.sendall(msg.encode())
            print(msg.encode())

            # time.sleep(0.01)



if __name__ == "__main__":
    main()
