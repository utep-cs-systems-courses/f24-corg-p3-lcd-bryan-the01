#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "led.h"
#include "stateMachine.h"
#include "p2_interrupt_handler.h"
#include "switches.h"
#include "../timerLib/clocksTimer.h"

u_int fontColor = COLOR_WHITE; // Default font color
u_int bgColor = COLOR_BLACK;    // Default background color

void drawUI(u_int fontColor, u_int bgColor) {
  clearScreen(bgColor);
  // Fill a rectangle with the font color
  fillRectangle(0, 0, 150, 10, fontColor);

  // Draw static text elements (e.g., a mode label or title)
  drawString5x7(5, 2, "Mode: ", bgColor, fontColor);

  // Additional UI elements can be added here
  drawString11x16(30, 50, "12:00:00", fontColor, bgColor);
}

int main() {
  WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer

  configureClocks();
  lcd_init();
  switch_init();             // Initialize switches
  init_leds();
  drawUI(fontColor, bgColor); // Draw initial UI

  __enable_interrupt();      // Enable global interrupts
  while (1) {
    if (switches & SW1) {  // S1: Green LED on
      turn_on_green_led();
      switches &= ~SW1; // Clear S1 bit
    }
    if (switches & SW2) {  // S2: Red LED on
      turn_on_red_led();
      switches &= ~SW2; // Clear S2 bit
    }
    if (switches & SW3) {  // S3: Both LEDs on
      turn_on_green_led();
      turn_on_red_led();
      switches &= ~SW3; // Clear S3 bit
    }
    if (switches & SW4) {  // S4: Both LEDs off
      turn_off_green_led();
      turn_off_red_led();
      u_int temp = bgColor;
      bgColor = fontColor;
      fontColor = temp;
      drawUI(fontColor, bgColor);
      __delay_cycles(5000);  
      switches &= ~SW4; // Clear S4 bit
    }
  }
  return 0;
}
