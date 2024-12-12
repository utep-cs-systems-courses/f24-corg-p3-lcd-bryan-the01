#include <msp430.h>
#include "lcdutils.h"
#include "stateMachine.h"

volatile short redrawScreen = 0;
extern void increment_time();
extern char currentMode;         // 0: Display Mode, 1: Edit Mode
extern char blinkFlag;           // Flag to toggle blinking in Edit Mode
extern void blinkDigit();

// Watchdog Timer interrupt handler
void __attribute__((interrupt(WDT_VECTOR))) wdt_c_handler() {
  static int secCount = 0;

  secCount++;
  // Handle Display Mode time increment
  if (secCount == 250) { // Update every second
    if (currentMode == 0) { // Only increment time in Display mode
      increment_time();
    }
    redrawScreen = 1; // Always update the screen if needed
    secCount = 0;
  }

  // Handle Edit Mode blinking
  if (currentMode == 1 && secCount % 50 == 0) { // Edit mode: blink digit
    blinkFlag = !blinkFlag;
    blinkDigit();
  }
  // Ensure CPU wakes up after interrupt
  __bic_SR_register_on_exit(CPUOFF); 
}
