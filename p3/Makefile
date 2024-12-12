# CPU and Compiler Flags
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os -I../lcdLib -I../timerLib
LDFLAGS         = -L../lib -L/opt/ti/msp430_gcc/include/

# Compiler and Assembler
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

# Make all
all: watch.elf
watch.elf: watch.o stateMachine.o led.o wdt_handler.o p2_interrupt_handler.o switches.o buzzer.o ../lcdLib/lcdutils.o ../lcdLib/lcddraw.o ../timerLib/clocksTimer.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lTimer -lLcd

# Files
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

# Files in lcdLib
../lcdLib/lcdutils.o: ../lcdLib/lcdutils.c ../lcdLib/lcdutils.h
	$(CC) $(CFLAGS) -c $< -o $@
../lcdLib/lcddraw.o: ../lcdLib/lcddraw.c ../lcdLib/lcddraw.h
	$(CC) $(CFLAGS) -c $< -o $@

# File in timerLib
../timerLib/clocksTimer.o: ../timerLib/clocksTimer.c ../timerLib/clocksTimer.h
	$(CC) $(CFLAGS) -c $< -o $@

# Load and Clean
load: watch.elf
	msp430loader.sh $^
clean:
	rm -f *.o *.elf ../lcdLib/*.o ../timerLib/*.o
