#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "buzzer.h"
#include "stateMachine.h"

char switch1_state,switch2_state,switch3_state,switch4_state;
char switch_state_changed;
switch_state = 0;

static char
switch_update_interrupt_sense(){
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);
  P2IES &= (p2val | ~SWITCHES);
  return p2val;
}

void
switch_init(){
  P2REN |= SWITCHES;
  P2IE = SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;
  switch_update_interrupt_sense();
}

void
switch_interrupt_handler(){
  char p2val = switch_update_interrupt_sense();

  switch1_state = (p2val & SW1) ? 0 : 1;
  switch2_state = (p2val & SW2) ? 0 : 1;
  switch3_state = (p2val & SW3) ? 0 : 1;
  switch4_state = (p2val & SW4) ? 0 : 1;

  if (switch1_state) //state switch
    switch_state = 1;
  if (switch2_state)
    switch_state = 2;
  if (switch3_state)
    switch_state = 3;
  if (switch4_state)
    switch_state = 4;

  switch_state_changed = 1; // button pressed
}
