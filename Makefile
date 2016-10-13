# Name: Makefile
# Author: Forrest Koran
# License: -TBD-

all:

hugs: TARGET = -DTARGET_HUGS
hugs: DEVICE = atmega8
hugs: PROGRAMMER = -c usbtiny -p m8
hugs: FUSES = -U hfuse:w:0xCA:m -U lfuse:w:0x3F:m
hugs: CLOCK = 16000000
hugs: clean hugs.hex

arduino: TARGET = -DTARGET_ARDUINO
arduino: DEVICE = atmega168
arduino: PROGRAMMER = -c usbtiny -p m168
arduino: FUSES = -U hfuse:w:0xCA:m -U lfuse:w:0x3F:m
arduino: CLOCK = 16000000
arduino: clean arduino.hex

OBJECTS    = bldc.o hal.o
FIXED_REGS = 

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -std=gnu99 -Wall -O3 $(FIXED_REGS) $(TARGET) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -g

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -c $< -o $@

%.lst: %.elf
	avr-objdump -h -S $< > $@
	avr-objdump -d $<

.c.s:
	$(COMPILE) -S $< -o $@

%.hex: %.elf
	rm -f $@.hex
	avr-objcopy -j .text -j .data -O ihex $< $@
	avr-size --format=avr --mcu=$(DEVICE) $<

hugs.elf: $(OBJECTS)
	$(COMPILE) -o $@ $(OBJECTS)

arduino.elf: $(OBJECTS)
	$(COMPILE) -o $@ $(OBJECTS)

lut_data.c:
	python gen_lut.py 96

clean:
	rm -f *.hex *.elf *.lst lut_data.c lut*.png $(OBJECTS)

cpp:
	$(COMPILE) -E bldc.c

