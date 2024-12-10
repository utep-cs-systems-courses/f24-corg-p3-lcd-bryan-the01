#include <msp430.h>
#include "stateMachine.h"
#include "led.h"
#include "switches.h"
#include "buzzer.h"
#include "lcdutils.h"
#include "lcdtypes.h"
#include "lcddraw.h"



char toggle_red(){
  static char state = 0;
  switch (state){
  case 0:
    red_on = 1;
    state = 1;
    break;
  case 1:
    red_on = 0;
    state = 0;
    break;
  }
  return 1;
}

char toggle_green(){
  static char state = 0;
  switch (state){
  case 0:
    green_on = 1;
    state = 1;
    break;
  case 1:
    green_on = 0;
    state = 0;
    break;
  }
  return 1;
}


void B1() {
  static char midday = 0;
  if(midday){  
  drawString8x12(102, 54, "pm", COLOR_WHITE, COLOR_BLACK);
  midday = 0;
  }
  else{
    drawString8x12(102, 54, "am", COLOR_WHITE, COLOR_BLACK);
    midday = 1;
  }
}  
void red_alarm_beep(){
  static char alarmCount = 0;
  static char alarmToggle = 1;
  if (++alarmCount == 25){
    if(alarmToggle)
      buzzer_set_period(500);
    else
      buzzer_set_period(0);
    alarmToggle = !alarmToggle;
    alarmCount = 0;
  }
}

void green_alarm_beep(){
  static char alarmCount = 0;
  static char alarmToggle = 1;
  if (++alarmCount == 150){
    if(alarmToggle)
      buzzer_set_period(700);
    else
      buzzer_set_period(0);
    alarmToggle = !alarmToggle;
    alarmCount = 0;
  }
}

void led_state_advance(){
  char changed = 0;
  static enum {R=0,G=1} color = G;
  switch (color){
  case R: changed = toggle_red(); color = G; break;
  case G: changed = toggle_green(); color = R; break;
  }
  led_changed = changed;
  led_update();
}

void switch_state_advance(){
  switch (switch_state){
  case 1:
    B1();
    break;
  case 2:
    green_alarm_beep();
    break;
  case 3:
    red_alarm_beep();
    break;
  case 4:
    buzzer_off();
    break;
  }
  led_update();
  //  switch_interrupt_handler();
}
