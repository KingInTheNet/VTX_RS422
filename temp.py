import serial
import struct
import sys
import time
bytes_to_send = struct.pack('B', 33)

ser = serial.Serial("COM3", baudrate=38400, timeout=1,xonxoff=False)
count = 0
start = time.time()
while (count < 1):
   data = b'\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18'
   # value = struct.unpack('B', data.encode('ASCII')[0:1])[0]
   # print(sys.getsizeof(value))
   # bytes_val = integer_val.to_bytes(data, 'little')
   ser.write(data)
#    resp = ser.readline()
#    print(resp)
   count = count + 1
end = time.time()
print("The time of execution of above program is :",
      (end-start) * 10**3, "ms")
# '0xA0': 0,
# '0xA1': 0,
# '0xA2': 0,
# '0xA3': 0,
# '0xA4': 0,
# '0xA5': 0,
# '0xA6': 0,
# '0xA7': 0,
# '0xA8': 0,
# '0xA9': 0,
# '0xAA': 0,
# '0xAD': 1,
# '0xAE': 1,
# '0xD1': 0,
# '0xD2': 0,
# '0xD3': 0,
# '0xDC': 0,
# '0xDE': 1,
# '0xDD': 1,
# '0xE0': 0,
# '0xED': 1

# 1 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 
# 0 


