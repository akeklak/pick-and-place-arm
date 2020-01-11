import serial
import time

def ints_to_byteInts(A, B, C):
    command = str(A) + "," + str(B) + "," + str(C)
    return bytes(command, 'utf8')

def open_connection():
    ser = serial.Serial('/dev/ttyACM0', 9600)
    return ser

def write_servo_pos(conn, A, B, C):
    positions = ints_to_byteInts(A, B, C)
    conn.write(positions)
    #conn.flushInput()

conn = open_connection()
time.sleep(2)
while True:
    position_1 = str(input("input pos 1: "))
    position_2 = str(input("input pos 2: "))
    position_3 = str(input("input pos 3: "))
    write_servo_pos(conn, position_1, position_2, position_3)
    time.sleep(2)
