# description #

Drive sinusoidal output signals on atmega8-based brushless motor controllers

# hardware #

http://www.readytoflyquads.com/rtf-hugesc-12amp 

# environment #

apt-get install python3 python-pip feh gcc-avr avr-libc avrdude
pip install numpy matplotlib

# build #

By default the firmware is configured for the Arduino Duemilnove or other atmega168-based arduino platforms. To build for an ESC target, edit Makefile and hal.h

vim lut.h
python gen_lut_SVM.py
feh lut_data.png

make clean; make; make lst; make flash
