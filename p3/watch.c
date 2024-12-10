#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "stateMachine.h"
#include "p2_interrupt_handler.h"
#include "switches.h"

u_int fontColor = COLOR_WHITE;  // Initial font color
u_int bgColor = COLOR_BLUE;     // Initial background color

int main() {
  WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
  configureClocks();
  lcd_init();
  switch_init();             // Initialize switches
  drawUI(COLOR_WHITE, COLOR_BLACK);

  while (1) {
    if (switch_state == 4) { // S4 pressed
      u_int temp = fontColor;
      fontColor = bgColor;
      bgColor = temp;

      drawUI(fontColor, bgColor);
      switch_state = 0;  // Reset state
    }
  }
  return 0;
}
void drawUI(u_int fontColor, u_int bgColor) {
  clearScreen(bgColor);
  // Fill a rectangle with the font color
  fillRectangle(0, 0, 150, 10, fontColor);

  // Draw static text elements (e.g., a mode label or title)
  drawString5x7(5, 2, "Mode: ", bgColor, fontColor);

  // Additional UI elements can be added here
  drawString11x16(30, 50, "12:00:00", fontColor, bgColor);
}
