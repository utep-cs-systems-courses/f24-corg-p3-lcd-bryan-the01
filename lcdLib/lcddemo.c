/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLACK);
  fillRectangle(0,0, 150, 10, COLOR_WHITE);
  drawString5x7(5, 2, "Mode: ", COLOR_BLACK, COLOR_WHITE);
  drawString5x7(100, 2, "Clock: ", COLOR_BLACK, COLOR_WHITE);
  drawString11x16(40, 50, "12:00", COLOR_WHITE, COLOR_BLACK);
  drawString8x12(102, 54, "am", COLOR_WHITE, COLOR_BLACK);
}
