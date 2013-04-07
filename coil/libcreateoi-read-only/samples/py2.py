import serial

ser = serial.Serial('/dev/ttyACM0')

while 1:
	print ser.readline()

ser.close()
