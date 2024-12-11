#include <msp430.h>
#include "lcdutils.h"
#include "stateMachine.h"

volatile short redrawScreen = 0;

// Watchdog Timer interrupt handler
void __attribute__((interrupt(WDT_VECTOR))) wdt_c_handler() {
  static int secCount = 0;

  secCount++;
  if (secCount == 500) { // 25 interrupts per second
    redrawScreen = 1; // Set flag to redraw the screen
    secCount = 0;
  }

  if (redrawScreen) {
    __bic_SR_register_on_exit(CPUOFF); // Clear CPUOFF bit to wake up CPU
  }
}
