import serial
import time

import socket
import select
import sys

def main():
    if len(sys.argv) < 3:
        # HOST = "13.125.21.41"
        # HOST = "192.168.0.3"
        HOST = "localhost"
        # PORT = 9901
        PORT = 9000
        print("Use default setting. {0}:{1}".format(HOST, PORT))
        # HOST = "132.145.113.171"
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

    i = 0

    # serial
    # ser = serial.Serial('/dev/cu.usbmodem14601', 38400)
    # ser = serial.Serial('/dev/ttyACM0', 38400)
    ser = serial.Serial('COM15', 38400)
    time.sleep(1)
    # print(ser)

    m_values = [0, 0, 0]
    a_values = [0, 0, 0]
    g_values = [0, 0, 0]
    y_values = [0, 0, 0]
    q_values = [0, 0, 0, 0]

    prev_value = [
            0,
            0, 0, 0,
            0, 0, 0,
            0, 0, 0,
            0, 0, 0,
            0, 0, 0, 0,
            0
            ]

    while True:

        # Get sensor values from arduino
        b = ser.readline()
        # print("1 - ")
        # print(b)
        string_b = b.decode()
        # print("2 - ")
        # print(string_b)
        string = string_b.rstrip()
        # print("3 - ")
        # print(string)

        v = string.split(' ')
        # print("4 - ")
        # print(v)
        if len(v) == 18 and v[0] == 's' and v[-1] == 'e':

            for i in range(1, len(v)-1):
                if v[i] is not ' ':
                    prev_value[i] = v[i]
                    # print(i)
                else:
                    print("Data has space..")
                    print(v[i])
                    v[i] = prev_value[i]

            for i in range(1, len(v)-1):
                print(v[i])

            try:
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
                q_values[0] = float(v[13])
                q_values[1] = float(v[14])
                q_values[2] = float(v[15])
                q_values[3] = float(v[16])
            except:
                print("error")

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


            # Display yaw, pitch, roll
            q0 = (q_values[0])
            qx = (q_values[1])
            qy = (q_values[2])
            qz = (q_values[3])
            # print("Yaw, Roll, Pitch: {0}, {1}, {2}\n".format(y, p, r))
            qert = "{0},{1},{2},{3}".format(q0, qx, qy, qz)


            msg = "{0},{1},{2},{3},{4},{5}".format(time_stamp, mag, acc, gyro, ypr, qert) # yrp = yaw, pitch, roll
            # print("5 - ")
            print(msg.encode())
            # MASTER_SOCK.sendall(msg.encode())

            try:
                # print(msg)
                MASTER_SOCK.sendall(msg.encode())
            except socket.error as msg:
                # print(socket.error)
                print(' ')
            # finally:
                # MASTER_SOCK.close()

            i = i + 1
            print(i)

        else:
            if len(v) != 18:
                print("len is not 18")
                print(len(v))
                continue
            elif v[0] != 's':
                print("start byte is not \'s\'!!")
                continue
            elif v[-1] != 'e':
                print("end byte is not \'e\'!!")
                continue
        time.sleep(0.05)



if __name__ == "__main__":
    main()
