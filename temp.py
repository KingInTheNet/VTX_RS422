import serial
import time
import struct
import threading


def write(bytes_to_send):
    while True:
        print ("Writing: ",  bytes_to_send)
        ser.write(str(bytes_to_send).encode())
        time.sleep(1)
        print ("Restart")
        ser.flush() #flush the buffer
    return 0

def listen():
    while True:
        try:
            print ("Attempt to Read")
            readOut = ser.readline().decode('ascii')
            time.sleep(1)
            print ("Reading: ", readOut) 
            break
        except:
            pass
    return 0


if __name__ =="__main__":
    ser = serial.Serial('/dev/ttyUSB1', 38400) 

    # readOut = 0   #chars waiting from laser range finder

    print ("Starting up")
    ser.close
    # connected = False
    # commandToSend = 1 # get the distance in mm
    # bytes_to_send = struct.pack('B', 33)
    # t1 = threading.Thread(target=listen)
    # t2 = threading.Thread(target=write, args=(bytes_to_send,))

    # t1.start()
    # t2.start()

    # t1.join()
    # t2.join()

    # print("Done!")