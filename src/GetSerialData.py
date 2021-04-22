import serial
import time
import csv
import struct

ser = serial.Serial(port='COM5',timeout=5,baudrate=9600)
ser.flushInput()

buf = []
with open("test_data.csv","a",newline='') as f:
    writer = csv.writer(f,delimiter=",")
    writer.writerow(["accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z"])
    buf.clear()
while True:
    ser_bytes = ser.read(4)
    ser_bytes = ser_bytes[-1: -len(ser_bytes)-1: -1]
    [decoded_bytes] = struct.unpack('f', ser_bytes)
    buf.append(decoded_bytes)
    print(decoded_bytes)
    if (len(buf) == 6):
        with open("test_data.csv","a",newline='') as f:
            writer = csv.writer(f)
            writer.writerow(buf)
            buf.clear()
