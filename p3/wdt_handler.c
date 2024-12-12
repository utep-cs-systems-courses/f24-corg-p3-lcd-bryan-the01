#include <msp430.h>
#include "lcdutils.h"
#include "stateMachine.h"

volatile short redrawScreen = 0; // Variable to determine if screen needs
                                 // be redrawn

extern void increment_time();    // Method to increment displayed time
extern char currentMode;         // 0 is Display mode, 1 is Edit mode
extern char blinkFlag;           // Flag to toggle blinking in Edit Mode
extern void blinkDigit();        // Method to blink current edit digit
                                 // only if blinkFlag is 1

// Watchdog Timer interrupt handler
void __attribute__((interrupt(WDT_VECTOR))) wdt_c_handler() {
  static int secCount = 0;

  secCount++;
  // Display mode
  if (secCount == 250) {      // Update every second(ish)
    if (currentMode == 0)     // Only increment time in Display mode
      increment_time();
    redrawScreen = 1;         // Always redraw screen to reflect time change
    secCount = 0;             // Reset sec count
  }

  // Edit mode
  if (currentMode == 1 && secCount % 50 == 0) { // Every 50 secCount 
    blinkFlag = !blinkFlag;                     // Toggle blink flag
    blinkDigit();                               // Blink current digit if flag is 1
  }
  
  __bic_SR_register_on_exit(CPUOFF); 
}
