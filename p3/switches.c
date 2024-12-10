#include "switches.h"

// Current switch state (1 = S1, 2 = S2, 3 = S3, 4 = S4)
char switch_state = 0;

// Update interrupt sense (trigger on state changes)
static char switch_update_interrupt_sense() {
  char p2val = P2IN; // Read the current state of Port 2

  // Configure interrupts to detect state changes
  P2IES |= (p2val & SWITCHES);   // If switch is up, detect transition to down
  P2IES &= (p2val | ~SWITCHES);  // If switch is down, detect transition to up

  return p2val;
}

// Initialize switches
void switch_init() {
  P2REN |= SWITCHES;     // Enable resistors for switches
  P2IE |= SWITCHES;      // Enable interrupts for switches
  P2OUT |= SWITCHES;     // Pull-up resistors
  P2DIR &= ~SWITCHES;    // Set switches as inputs

  switch_update_interrupt_sense();
}

// Interrupt handler for switches
void switch_interrupt_handler() {
  char p2val = switch_update_interrupt_sense(); // Update interrupt sense

  // Determine which switch is pressed
  if (!(p2val & SW1)) switch_state = 1;
  else if (!(p2val & SW2)) switch_state = 2;
  else if (!(p2val & SW3)) switch_state = 3;
  else if (!(p2val & SW4)) switch_state = 4;
  else switch_state = 0; // No switch pressed
}
