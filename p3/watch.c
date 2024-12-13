#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "led.h"
#include "stateMachine.h"
#include "p2_interrupt_handler.h"
#include "switches.h"
#include "../timerLib/clocksTimer.h"
#include "buzzer.h"

u_int fontColor = COLOR_WHITE;    // Font color
u_int bgColor = COLOR_BLACK;      // Background color
char daytimeToggle = 0;           // 0 for night mode, 1 for Day mode
unsigned int h1 = 1, h2 = 2;      // Hour digits (HH:MM:SS format)
unsigned int m1 = 0, m2 = 0;      // Minute digits
unsigned int s1 = 0, s2 = 0;      // Second digits
char currentMode = 0;             // 0 for Display mode, 1 for Edit mode
char blinkFlag = 0;               // Blink flag for current edit digit
char editDigit = 0;               // 0 for h1, 1 for h2,..etc

// Method to draw user interface
void drawUI(u_int fontColor, u_int bgColor) {
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

  // Display each digit in its place
  drawChar11x16(24, 50, '0' + h1, fontColor, bgColor); // Hour tens place
  drawChar11x16(36, 50, '0' + h2, fontColor, bgColor); // Hour ones place
  drawChar11x16(48, 50, ':', fontColor, bgColor);      // Separator
  drawChar11x16(60, 50, '0' + m1, fontColor, bgColor); // Minute tens place
  drawChar11x16(72, 50, '0' + m2, fontColor, bgColor); // Minute ones place
  drawChar11x16(84, 50, ':', fontColor, bgColor);      // Separator
  drawChar11x16(96, 50, '0' + s1, fontColor, bgColor); // Second tens place
  drawChar11x16(108, 50, '0' + s2, fontColor, bgColor);// Second ones place
}

// Updates graphical display of time only, actual 
// time variable increased in incerement_time(function)
void updateTime(u_int fontColor, u_int bgColor) {
  if (currentMode == 1) {   // if in Edit mode, returns without updating time
    return;
  }
  drawChar11x16(24, 50, '0' + h1, fontColor, bgColor); // Hour tens place
  drawChar11x16(36, 50, '0' + h2, fontColor, bgColor); // Hour ones place
  drawChar11x16(48, 50, ':', fontColor, bgColor);      // Separator
  drawChar11x16(60, 50, '0' + m1, fontColor, bgColor); // Minute tens place
  drawChar11x16(72, 50, '0' + m2, fontColor, bgColor); // Minute ones place
  drawChar11x16(84, 50, ':', fontColor, bgColor);      // Separator
  drawChar11x16(96, 50, '0' + s1, fontColor, bgColor); // Second tens place
  drawChar11x16(108, 50, '0' + s2, fontColor, bgColor);// Second ones place
}

// Toggles day/night mode, writes mode on top left
// color inversion done in SW4
void daytime_toggle() {
  if (daytimeToggle) {
    daytimeToggle = 0; // Switch to Night mode
    drawString5x7(100, 2, "Night", bgColor, fontColor);
  } else {
    daytimeToggle = 1; // Switch to Day mode
    drawString5x7(100, 2, "  Day", bgColor, fontColor);
  }
}

// Increments time in 
void increment_time() {
  if (currentMode == 1) return; // Edit mode needs no update
  
  // Seconds increment
  s2++;
  if (s2 == 10) {
    s2 = 0;
    s1++;}
  if (s1 == 6) {
    s1 = 0;
    m2++;}
  
  // Minutes increment
  if (m2 == 10) {               // Minutes increment
    m2 = 0;
    m1++;}
  if (m1 == 6) {
    m1 = 0;
    h2++;}
  
  // Hours increment
  if ((h1 == 1 && h2 == 3) || (h1 == 0 && h2 == 10)) {
    h1 = (h1 == 1) ? 0 : 1;
    h2 = (h1 == 0) ? 1 : 0;
  }
}

// Toggles between edit and display mode
void toggleMode() {
  editDigit = 0;
  currentMode = !currentMode;
  if (currentMode) 
    drawString5x7(37, 2, " Edit   ", bgColor, fontColor);
  else {
    drawString5x7(37, 2, " Display", bgColor, fontColor);
    blinkFlag = 0; // Stop blinking when leaving Edit mode
    redrawScreen = 1; // Force a screen redraw to refresh the time display
  }
}

// Will alternate between digit and ' ' to simulate blinking
// is used in wdt_handler.c
void blinkDigit() {
  
  // Only blink on Edit mode
  if (currentMode == 1) {
    
    // Hide current digits
    if (blinkFlag) {
      switch (editDigit) {
      case 0: drawChar11x16(24, 50, ' ', fontColor, bgColor); break; // Clear h1
      case 1: drawChar11x16(36, 50, ' ', fontColor, bgColor); break; // Clear h2
      case 2: drawChar11x16(60, 50, ' ', fontColor, bgColor); break; // Clear m1
      case 3: drawChar11x16(72, 50, ' ', fontColor, bgColor); break; // Clear m2
      case 4: drawChar11x16(96, 50, ' ', fontColor, bgColor); break; // Clear s1
      case 5: drawChar11x16(108, 50, ' ', fontColor, bgColor); break; // Clear s2
      }
      
      // Unhide digits
    } else {
      switch (editDigit) {
      case 0: drawChar11x16(24, 50, '0' + h1, fontColor, bgColor); break;
      case 1: drawChar11x16(36, 50, '0' + h2, fontColor, bgColor); break;
      case 2: drawChar11x16(60, 50, '0' + m1, fontColor, bgColor); break;
      case 3: drawChar11x16(72, 50, '0' + m2, fontColor, bgColor); break;
      case 4: drawChar11x16(96, 50, '0' + s1, fontColor, bgColor); break;
      case 5: drawChar11x16(108, 50, '0' + s2, fontColor, bgColor); break;
      }}}}

// Will switch the current digit thats being modified
// new modified digit will be blinkin
void changeBlinkDigit() {
  switch (editDigit) {
  case 0: drawChar11x16(24, 50, '0' + h1, fontColor, bgColor); break;
  case 1: drawChar11x16(36, 50, '0' + h2, fontColor, bgColor); break;
  case 2: drawChar11x16(60, 50, '0' + m1, fontColor, bgColor); break;
  case 3: drawChar11x16(72, 50, '0' + m2, fontColor, bgColor); break;
  case 4: drawChar11x16(96, 50, '0' + s1, fontColor, bgColor); break;
  case 5: drawChar11x16(108, 50, '0' + s2, fontColor, bgColor); break;
  }
  
  // Force a redraw and only between the 6b digits
  editDigit = (editDigit + 1) % 6;
  redrawScreen = 1;
}

// Will inceremnt internal value of h1, h2, ... etc
void incrementEditDigit() {
  if (currentMode == 1){
    switch (editDigit) {
    case 0: h1 = (h1 == 1) ? 0 : h1 + 1; break;
    case 1: h2 = (h1 == 1 && h2 == 2) ? 0 : h2 + 1; break;
    case 2: m1 = (m1 == 5) ? 0 : m1 + 1; break;
    case 3: m2 = (m2 == 9) ? 0 : m2 + 1; break;
    case 4: s1 = (s1 == 5) ? 0 : s1 + 1; break;
    case 5: s2 = (s2 == 9) ? 0 : s2 + 1; break;
    }
    redrawScreen = 1;
  }
}


int main() {
  WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
  configureClocks();         // Initialize everything
  lcd_init();
  switch_init();             
  buzzer_init();
  init_leds();
  drawUI(fontColor, bgColor); // Draw initial UI
  enableWDTInterrupts();      // Enable interrupts
  __enable_interrupt();
  while (1) {
    // Redraw time
    if (redrawScreen) {
      redrawScreen = 0;               // Clear the flag
      updateTime(fontColor, bgColor); // Redraw only the time
    }
    
    // S1 pressed
    // Will go into Edit mode if in Display mode, and vice versa
    if (switches & SW1) {
      toggleMode();
      buzzer_set_period(1450);
      __delay_cycles(250000);
      buzzer_off();
      drawString8x12(5, 120, "S1 pressed", fontColor, bgColor);
      switches &= ~SW1;
    }

    // S2 pressed
    // If in edit mode, it will change position of digit being edited
    if (switches & SW2) {
      buzzer_set_period(1300);
      __delay_cycles(250000);
      buzzer_off();
      if (currentMode == 1)
	changeBlinkDigit();
      drawString8x12(5, 120, "S2 pressed", fontColor, bgColor);
      switches &= ~SW2; // Clear S2 bit
    }

    // S3 pressed
    // If in edit mode, it will increment the value of the edited digit by one
    if (switches & SW3) {
      buzzer_set_period(1150);
      __delay_cycles(250000);
      buzzer_off();
      incrementEditDigit();
      redrawScreen = 1;
      drawString8x12(5, 120, "S3 pressed", fontColor, bgColor);
      switches &= ~SW3;
    }

    // S4 is pressed
    // WIll invert the colors of the screen
    if (switches & SW4) {
      u_int temp = bgColor;
      bgColor = fontColor;
      fontColor = temp;
      buzzer_set_period(1000);
      __delay_cycles(250000);
      buzzer_off();
      drawUI(fontColor, bgColor);
      daytime_toggle();
      drawString8x12(5, 120, "S4 pressed", fontColor, bgColor);
      buzzer_off();
      switches &= ~SW4;
    }
    __bis_SR_register(LPM0_bits + GIE);
    
  }
  return 0;
}
