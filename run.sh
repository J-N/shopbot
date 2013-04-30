#!/bin/bash
ROB=$(ls /dev/ttyUSB0)
ROB=${ROB:0:1}
if [ "$ROB" == "/" ]; then
	chmod 777 /dev/ttyUSB0
	navigation/src/navigation /dev/ttyUSB0 &
else
	chmod 777 /dev/ttyUSB1
	navigation/src/navigation /dev/ttyUSB1 &
fi
	cd QR
	./run.sh &
