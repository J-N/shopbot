import time

f=open("/dev/QRComms", "wb")
time.sleep(20)
f.write("22\n")
