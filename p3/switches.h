#ifndef SWITCHES_H
#define SWITCHES_H

#include <msp430.h>

// Define switches
#define SW1 BIT0
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3
#define SWITCHES (SW1 | SW2 | SW3 | SW4)

// External declaration for switch state
extern char switch_state;

// Function prototypes
void switch_init();                // Initialize switches
void switch_interrupt_handler();   // Handle switch interrupts

#endif // SWITCHES_H
