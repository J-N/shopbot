import time
import urllib2
import sys

f=open("/dev/pathComms", "wb")
f.write("H\n")
f.flush()
