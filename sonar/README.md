shopbot
=======

We need to patch the kernel so that we can communicate with the sonar sensor

Instructions:
1.) run sudo apt-get install dkms 
2.) copy kernel_patch/KERNELVERSION/cdcacm-0.1 to /usr/src
3.) run sudo /usr/src/cdcacm-0.1/install.sh
4.) connect the MSP430 to the usb port
