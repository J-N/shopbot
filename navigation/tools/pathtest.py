import time
import urllib2
url='http://kinetic.hp.af.cm/shopbot_list.php'
req = urllib2.Request(url)
res = urllib2.urlopen(req)
str = res.read()
str2 = str[30:]
st3 = st2

f=open("/dev/pathComms", "wb")
while 1:
	item=input('Please enter an item ID: ')
	quant=input('Please enter a quality ID: ')
	f.write("%s" % item)
	f.write("|")
	f.write("%s\n" % quant)
	f.flush()
