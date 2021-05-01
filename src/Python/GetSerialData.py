import serial
import time
import csv
import struct

ser = serial.Serial(port='COM5',timeout=5,baudrate=9600)
ser.flushInput()

buf = []
with open("test_data.csv","a",newline='') as f:
    writer = csv.writer(f,delimiter=",")
    writer.writerow(["timestamp,accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z"])
    buf.clear()
ser.write(b'start sending me data!')
while True:
    decoded_bytes = []
    for i in range(7):
        ser_bytes = ser.read(4)
        decoded_bytes.append(float(struct.unpack('<f', ser_bytes)[0]))
    # buf.append(decoded_bytes)
    print(decoded_bytes)
    if (len(decoded_bytes) == 7):
        with open("test_data.csv","a",newline='') as f:
            writer = csv.writer(f)
            writer.writerow(decoded_bytes)
            decoded_bytes.clear()
