#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

int
main()
{
  configureClocks();
  lcd_init();
  switch_init();
  led_init();
  buzzer_init();
  enableWDTInterrupts();

  or_sr(0x18);
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLACK);
  fillRectangle(0,0, 150, 10, COLOR_WHITE);
  drawString5x7(5, 2, "Mode: ", COLOR_BLACK, COLOR_WHITE);
  drawString5x7(100, 2, "Clock: ", COLOR_BLACK, COLOR_WHITE);
  drawString11x16(40, 50, "12:00", COLOR_WHITE, COLOR_BLACK);
  drawString8x12(102, 54, "am", COLOR_WHITE, COLOR_BLACK);
}
