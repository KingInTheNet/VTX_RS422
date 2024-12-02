import serial
import struct
import binascii
import os
import time

CMD_code = {
  '0xA0': 0,
  '0xA1': 0,
  '0xA2': 0,
  '0xA3': 0,
  '0xA4': 0,
  '0xA5': 0,
  '0xA6': 0,
  '0xA7': 0,
  '0xA8': 0,
  '0xA9': 0,
  '0xAA': 0,
  '0xAD': 1,
  '0xAE': 1,
  '0xD1': 0,
  '0xD2': 0,
  '0xD3': 0,
  '0xDC': 0,
  '0xDE': 1,
  '0xDD': 1,
  '0xE0': 0,
  '0xED': 1
}
processing = False
B_type = False
# final_data = False


def random_hex_string(length=24):
    return os.urandom(length)

if __name__ =="__main__":
   bytes_to_send = struct.pack('B', 33)
   # big_data = random.randint(100, size=(23))
   # ser = serial.Serial("COM6", baudrate=38400, timeout=1)
   # while True:
   #    data = ser.read(1)
   #    try:
   #       print(int(data))
   #    except:
   #       continue
   count = 0
   ser = serial.Serial("COM3", baudrate=38400, timeout=1)

   # random_bytes = random_hex_string()
   # print(type(random_bytes))  # <class 'bytes'>

   # hex_string = random_bytes.hex()
   # print(type(hex_string))    # <class 'str'>
   # hex_array = bytes.fromhex(hex_string)
   rList = [1, 2, 3, 4, 5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24]
   hex_array = bytearray(rList)
   # print(hex_array)    
   # for element in hex_array:
   #     print(element)
      #  print("\n")
   while(count < 75):
      start = time.time()
      data = ser.read(1)
      # time.sleep(0.01)
      byte ="0x" + str(binascii.hexlify(data).upper()).split("'")[1]
       
      # print(data)
      # print(byte)
      if not processing and byte in CMD_code.keys():
         processing = True
         B_type = CMD_code[byte]
         print(CMD_code[byte])
         # print('writing back')
         ser.write(data)
      elif processing and byte == '0xAC':
         if B_type:
            # print("dumping telemetry")
            # print(hex_array)
            ser.write(hex_array)
            # ser.write(data)
         else:
            ser.write(data)
         processing = False
         B_type = False
         # ser.write(data)
      elif processing:
         # print('writing back')
         
         ser.write(data)
      

      # resp = int.from_bytes(data, "little")
      # print(data)
      # resp_int = resp.hex()
      # print(int.from_bytes(data, "little"))
      # try:
      #    print(data.encode())
      # except:
      #    pass
      
      

      
      # print(data)
      # ser.reset_output_buffer()
      # if (not data.decode(errors='ignore') == "b''"):
      count = count + 1
      end = time.time()
      if processing:
         print("The time of execution of above program is :",(end-start) * 10**3, "ms")
   ser.close()

