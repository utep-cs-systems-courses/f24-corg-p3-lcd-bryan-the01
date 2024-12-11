#ifndef SWITCHES_H
#define SWITCHES_H

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES 15 // Represents all switches

extern int switches; // Bitfield representing switch states

void switch_init();
void switch_interrupt_handler();

#endif // SWITCHES_H
