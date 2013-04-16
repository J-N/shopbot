import time

f=open("/dev/pathComms", "wb")
while 1:
	item=input('Please enter an item ID: ')
	f.write("%s\n" % item)
	f.flush()
