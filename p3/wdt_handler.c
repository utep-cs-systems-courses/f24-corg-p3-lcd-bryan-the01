#include <msp430.h>
#include "lcdutils.h"
#include "stateMachine.h"

volatile short redrawScreen = 0;
extern void increment_time();

// Watchdog Timer interrupt handler
void __attribute__((interrupt(WDT_VECTOR))) wdt_c_handler() {
  static int secCount = 0;
  extern void increment_time();

  secCount++;
  if (secCount == 250) { // 250
    increment_time();
    redrawScreen = 1; // Set flag to redraw the screen
    secCount = 0;
  }
  if (currentMode == 1 && secCount % 50 == 0) { // Edit Mode: Toggle blinkFlag every half second
    blinkFlag = !blinkFlag;
    redrawScreen = 1;
  }
  // Ensure CPU wakes up after interrupt
  __bic_SR_register_on_exit(CPUOFF); 
}
