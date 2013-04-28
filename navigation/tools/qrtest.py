import time

f=open("/dev/QRComms", "wb")
while 1:
	time.sleep(20)
	f.write("22\n")
