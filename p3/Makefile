# Makefile for new project

# CPU and Compiler Flags
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os -I../h
LDFLAGS         = -L../lib -L/opt/ti/msp430_gcc/include/

# Compiler and Assembler
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

# Top-Level Targets
all: watch.elf

# Rules
watch.elf: watch.o stateMachine.o led.o wdt_handler.o p2_interrupt_handler.o switches.o buzzer.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lTimer -lLcd

# Compile Object Files
watch.o: watch.c
	$(CC) $(CFLAGS) -c $< -o $@

stateMachine.o: stateMachine.c stateMachine.h
	$(CC) $(CFLAGS) -c $< -o $@

led.o: led.c led.h
	$(CC) $(CFLAGS) -c $< -o $@

switches.o: switches.c switches.h
	$(CC) $(CFLAGS) -c $< -o $@

wdt_handler.o: wdt_handler.c
	$(CC) $(CFLAGS) -c $< -o $@

p2_interrupt_handler.o: p2_interrupt_handler.c
	$(CC) $(CFLAGS) -c $< -o $@

buzzer.o: buzzer.c buzzer.h
	$(CC) $(CFLAGS) -c $< -o $@

# Load Program
load: watch.elf
	msp430loader.sh $^

# Clean
clean:
	rm -f *.o *.elf
