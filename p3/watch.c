#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "led.h"
#include "stateMachine.h"
#include "p2_interrupt_handler.h"
#include "switches.h"
#include "../timerLib/clocksTimer.h"
#include "buzzer.h"

u_int fontColor = COLOR_WHITE;    // Default font color
u_int bgColor = COLOR_BLACK;      // Default background color
char daytimeToggle = 0;           // 0 for "Night", 1 for "Day"
unsigned int h1 = 1, h2 = 2;      // Hour digits 1 and 2 (HH:MM:SS format)
unsigned int m1 = 0, m2 = 0;      // Minute digits
unsigned int s1 = 0, s2 = 0;      // Second digits
char currentMode = 0;             // 0 for Display mode, 1 for Edit mode
char blinkFlag = 0;               // Blink flag for current edit digit

void drawUI(u_int fontColor, u_int bgColor) {
  //Clear screen and create graphical UI
  clearScreen(bgColor);
  fillRectangle(0, 0, 150, 10, fontColor);
  drawString5x7(5, 2, "Mode: ", bgColor, fontColor);
  if(daytimeToggle)
    drawString5x7(100, 2, "  Day", bgColor, fontColor);
  else
    drawString5x7(100, 2, "Night", bgColor, fontColor);
  if(currentMode)
    drawString5x7(37, 2, " Edit   ", bgColor, fontColor);
  else
    drawString5x7(37, 2, " Display", bgColor, fontColor);

  // Display each digit in its respective place
  drawChar11x16(24, 50, '0' + h1, fontColor, bgColor); // Hour tens place
  drawChar11x16(36, 50, '0' + h2, fontColor, bgColor); // Hour ones place
  drawChar11x16(48, 50, ':', fontColor, bgColor);      // Separator
  drawChar11x16(60, 50, '0' + m1, fontColor, bgColor); // Minute tens place
  drawChar11x16(72, 50, '0' + m2, fontColor, bgColor); // Minute ones place
  drawChar11x16(84, 50, ':', fontColor, bgColor);     // Separator
  drawChar11x16(96, 50, '0' + s1, fontColor, bgColor);// Second tens place
  drawChar11x16(108, 50, '0' + s2, fontColor, bgColor);// Second ones place
}

void modeBuzz(){
  static char alarmCount = 0;
  static char alarmToggle = 0;
  if (++alarmCount == 25) {
    if (alarmToggle)
      buzzer_set_period(700);
    else
      buzzer_set_period(500);
    alarmToggle = !alarmToggle;
    alarmCount = 0;
  }
}

void updateTime(u_int fontColor, u_int bgColor) {
  if (currentMode == 1 && !blinkFlag) {
    // Hide h1 by drawing it in the background color
    drawChar11x16(24, 50, ' ', bgColor, bgColor);
  } else {
    // Draw h1 normally
    drawChar11x16(24, 50, '0' + h1, fontColor, bgColor);
  }

  // Redraw each digit in its respective place
  // drawChar11x16(24, 50, '0' + h1, fontColor, bgColor); // Hour tens place
  drawChar11x16(36, 50, '0' + h2, fontColor, bgColor); // Hour ones place
  drawChar11x16(48, 50, ':', fontColor, bgColor);      // Separator
  drawChar11x16(60, 50, '0' + m1, fontColor, bgColor); // Minute tens place
  drawChar11x16(72, 50, '0' + m2, fontColor, bgColor); // Minute ones place
  drawChar11x16(84, 50, ':', fontColor, bgColor);      // Separator
  drawChar11x16(96, 50, '0' + s1, fontColor, bgColor);// Second tens place
  drawChar11x16(108, 50, '0' + s2, fontColor, bgColor);// Second ones place
}

void daytime_toggle() {
  if (daytimeToggle) {
    daytimeToggle = 0; // Switch to "Night"
    drawString5x7(100, 2, "Night", bgColor, fontColor);
  } else {
    daytimeToggle = 1; // Switch to "Day"
    drawString5x7(100, 2, "  Day", bgColor, fontColor);
  }
}

void increment_time() {
  if (currentMode == 1) return; // Edit mode needs no update
  s2++;                         // Seconds increment
  if (s2 == 10) {
    s2 = 0;
    s1++;
  }
  if (s1 == 6) {
    s1 = 0;
    m2++;
  }
  if (m2 == 10) {               // Minutes increment
    m2 = 0;
    m1++;
  }
  if (m1 == 6) {
    m1 = 0;
    h2++;
  }
  // Hours increment 12-hour format
  if ((h1 == 1 && h2 == 3) || (h1 == 0 && h2 == 10)) {
    h1 = (h1 == 1) ? 0 : 1;
    h2 = (h1 == 0) ? 1 : 0;
  }
}

// Toggles between edit and display mode
void toggleMode() {
  currentMode = !currentMode;
  if (currentMode) {
    drawString5x7(37, 2, " Edit   ", bgColor, fontColor);
  } else {
    drawString5x7(37, 2, " Display", bgColor, fontColor);
  }
}

int main() {
  WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer

  configureClocks();
  lcd_init();
  switch_init();             // Initialize switches
  buzzer_init();
  init_leds();
  drawUI(fontColor, bgColor); // Draw initial UI
  enableWDTInterrupts();
  __enable_interrupt();      // Enable global interrupts
  while (1) {
    // Prioritize time redraw
    if (redrawScreen) {
      redrawScreen = 0; // Clear the flag
      updateTime(fontColor, bgColor); // Redraw only the time
    }
    if (switches & SW1) {  // S1: Green LED on
      toggleMode();
      // turn_on_green_led();
      buzzer_set_period(1450);
      __delay_cycles(250000);
      buzzer_off();
      drawString8x12(5, 120, "S1 pressed", fontColor, bgColor); // Update button status
      switches &= ~SW1; // Clear S1 bit
    }
    if (switches & SW2) {  // S2: Red LED on
      //      turn_on_red_led();
      // Beep briefly
      buzzer_set_period(1300);
      __delay_cycles(250000);
      buzzer_off();
      drawString8x12(5, 120, "S2 pressed", fontColor, bgColor);
      switches &= ~SW2; // Clear S2 bit
    }
    if (switches & SW3) {  // S3: Both LEDs on
      //turn_on_green_led();
      // turn_on_red_led();
      buzzer_set_period(1150);
      __delay_cycles(250000);
      buzzer_off();
      drawString8x12(5, 120, "S3 pressed", fontColor, bgColor); // Update button status
      switches &= ~SW3; // Clear S3 bit
    }
    if (switches & SW4) {  // S4: Both LEDs off
      //turn_off_green_led();
      //turn_off_red_led();
      //      drawString8x12(5,120, "S4", fontColor, bgColor);
      u_int temp = bgColor;
      bgColor = fontColor;
      fontColor = temp;
      buzzer_set_period(1000);
      __delay_cycles(250000);
      buzzer_off();
      drawUI(fontColor, bgColor);
      daytime_toggle();
      drawString8x12(5, 120, "S4 pressed", fontColor, bgColor);
      //      __delay_cycles(250000); // Short delay (~250ms at 1 MHz clock)
      buzzer_off();           // Turn off the buzzer
      switches &= ~SW4; // Clear S4 bit
    }
    __bis_SR_register(LPM0_bits + GIE); // Enter low-power mode with interrupts enabled
    
  }
  return 0;
}
