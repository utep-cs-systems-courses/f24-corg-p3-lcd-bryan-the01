#include <msp430.h>
#include "switches.h"
#include "led.h"

int switches = 0; // Bitfield for switch states

static char switch_update_interrupt_sense() {
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);   // Sense high-to-low transitio
  P2IES &= (p2val | ~SWITCHES);  // Sense low-to-high transition
  return p2val;
}

void switch_init() {
  P2REN |= SWITCHES;    // Enable resistors
  P2IE |= SWITCHES;     // Enable interrupts
  P2OUT |= SWITCHES;    // Pull-up resistors
  P2DIR &= ~SWITCHES;   // Set switches as inputs
  switch_update_interrupt_sense();
}

void switch_interrupt_handler() {
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES; // Update global switches variable
}
