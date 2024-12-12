#include <msp430.h>
#include "lcdutils.h"
#include "stateMachine.h"

volatile short redrawScreen = 0;
extern void increment_time();
extern char currentMode;         // 0: Display Mode, 1: Edit Mode
extern char blinkFlag;           // Flag to toggle blinking in Edit Mode

// Watchdog Timer interrupt handler
void __attribute__((interrupt(WDT_VECTOR))) wdt_c_handler() {
  static int secCount = 0;

  secCount++;
  // Handle Display Mode time increment
  if (currentMode == 0) { // Only increment time in Display Mode
    if (secCount == 250) { // Approximately 1 second
      increment_time();    // Increment the clock time
      redrawScreen = 1;    // Set flag to redraw the screen
      secCount = 0;        // Reset the counter
    }
  }

  // Handle Edit Mode blinking
  if (currentMode == 1) { // Edit Mode: Toggle blink every half second
    if (secCount % 125 == 0) { // Approximately 0.5 second
      blinkFlag = !blinkFlag;  // Toggle the blink flag
      redrawScreen = 1;        // Indicate screen update required
    }
  }
  // Ensure CPU wakes up after interrupt
  __bic_SR_register_on_exit(CPUOFF); 
}
