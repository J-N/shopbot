import time
import urllib2
import sys

dic = {'1':42, '2':23, '3':44, '4':23}
while 1:
	url='http://kinetic.hp.af.cm/shopbot_list.php'
	req = urllib2.Request(url)
	res = urllib2.urlopen(req)
	raw = res.read()
	str2 = raw[30:]
	pos = str2.find("</body")
	str3 = str2[:pos]
	str3=str3.replace("<br/>"," ")
	items=str3.split()
	if len(items) == 0:
		continue
	print "New order detected:"
	for i in xrange(0,len(items),2):
		quant=items[i]
		item=items[i+1]
		print "item: " + item + " translated itemid: "+str(dic[item]) + " quanitiy " +quant
	sys.exit(0)

f=open("/dev/pathComms", "wb")
while 1:
	item=input('Please enter an item ID: ')
	quant=input('Please enter a quality ID: ')
	f.write("%s" % item)
	f.write("|")
	f.write("%s\n" % quant)
	f.write("H\n")
	f.flush()
