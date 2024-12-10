#include <msp430.h>
#include "led.h"
#include "stateMachine.h"

unsigned char red_on = 0,green_on = 0;
unsigned char led_changed = 0;
char switch2_state,switch3_state;
char switch_state_changed;

static char redLED[] = {0,LED_RED},greenLED[] = {0,LED_GREEN};

void led_init(){
  P1DIR |= LEDS;
  led_changed = 1;
  switch_state_changed = 1;
  led_update();
}
void led_update(){
  char ledFlags;

  if (led_changed && switch_state == 1){
    ledFlags = greenLED[green_on] | redLED[red_on]; //intial state
    P1OUT &= (0xff^LEDS) | ledFlags;
    P1OUT |= ledFlags;
    led_changed = 0;
  }

  if (switch_state_changed && switch_state == 2){
    ledFlags &= ~LED_GREEN;
    ledFlags |= switch2_state ? 0 : LED_RED;

    P1OUT &= (0xff^LEDS) | ledFlags;
    P1OUT |= ledFlags;
  }

  if (switch_state_changed && switch_state == 3){ //turn on red and turn green off
    ledFlags &= ~LED_RED;
    ledFlags |= switch3_state ? 0 : LED_GREEN; //if button is pressed turn red off temporarily

    P1OUT &= (0xff^LEDS) | ledFlags;
    P1OUT |= ledFlags;
  }

  if (switch_state_changed && switch_state == 4){
    ledFlags &= ~LED_RED; //turn off leds
    ledFlags &= ~LED_GREEN;

    P1OUT &= (0xff^LEDS) | ledFlags;
    P1OUT |= ledFlags;
  }
  switch_state_changed = 0;
}
