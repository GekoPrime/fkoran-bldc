# fkoran-bldc #

Drive sinusoidal output signals on atmega8-based brushless motor controllers

# Hardware #

http://www.readytoflyquads.com/rtf-hugesc-12amp

The Arduino Duemilnove is provided as a debug target. Output waveforms will be on pins 7-12, with debug trace on pin 13

# Environment #

apt-get install python python-pip feh gcc-avr avr-libc avrdude

pip install numpy matplotlib

# Build #

make hugs

make arduino

feh *.png
