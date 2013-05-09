import time
import urllib2
import sys

f=open("/dev/pathComms", "wb")

while 1:
	item=input('Please enter an item ID: ')
	quant=input('Please enter a quality ID: ')
	f.write("%s" % item)
	f.write("|")
	f.write("%s\n" % quant)
#	f.write("H\n")
	f.flush()
