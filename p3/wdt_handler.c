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
  if (secCount == 50) { // 25 interrupts per second
    increment_time();
    redrawScreen = 1; // Set flag to redraw the screen
    secCount = 0;
  }

  if (redrawScreen) {
    __bic_SR_register_on_exit(CPUOFF); // Clear CPUOFF bit to wake up CPU
  }
}
