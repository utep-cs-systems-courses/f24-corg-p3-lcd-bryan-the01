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
    if (switches & SW4) {          // Check if S4 is pressed
      toggle_green_led();
      u_int temp = fontColor;
      fontColor = bgColor;       // Swap colors
      bgColor = temp;
      drawUI(fontColor, fontColor); // Redraw UI with new colors
      toggle_red_led();          // Debug: Toggle Red LED
      switches &= ~SW4;          // Clear the S4 bit to acknowledge the press
    }
  }
  return 0;
}
