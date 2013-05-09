from serial import *
from threading import Thread

last_received = ''

def receiving(ser):
	global last_received
	buffer = ''

	while True:
		buffer += ser.read(ser.inWaiting())
		if '\n' in buffer:
			print buffer
			last_recieved, buffer = buffer.split('\n')[-2]
			print last_received

if __name__ == '__main__':
	ser = Serial(
	port='/dev/ttyACM0',
	baudrate=9600,
#	bytesize=EIGHBITS,
#	parity=PARITY_NONE,
#	stopbits=STOPBITS_ONE,
	timeout=0.1,
	xonxoff=0,
	interCharTimeout=None
)
Thread(target=receiving, args=(ser,)).start()
