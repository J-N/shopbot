#!/bin/bash
apt-get install libboost-serialization-dev
apt-get install kernel-headers-`uname -r`
cd navigation/
make
cd ../
cd QR
./build.sh
cd ../
cd OP
./build.sh
cd ../
VERSION=$(uname -a | awk '{print $3}')
VERSION=${VERSION:0:1}

if [ "$VERSION" -eq "3" ]; then
	cd sonar/kernel_patch/linux-3.0
else
	cd sonar/kernel_patch/linux-2.6
fi
	cp -R cdcacm-0.1 /usr/src
	cd /usr/src/cdcacm-0.1
	./install.sh
