import serial
import struct
bytes_to_send = struct.pack('B', 33)

ser = serial.Serial("/dev/ttyUSB0", baudrate=38400, timeout=1)
while True:
   data = ser.readline()
   print(data)