#include <msp430.h>
#include "led.h"

// Initialize LED pins
void init_leds() {
  P1DIR |= (RED_LED | GREEN_LED); // Set P1.0 and P1.6 as output
  P1OUT &= ~(RED_LED | GREEN_LED); // Turn off both LEDs initially
}

// Turn on Red LED
void turn_on_red_led() {
  P1OUT |= RED_LED;
}

// Turn off Red LED
void turn_off_red_led() {
  P1OUT &= ~RED_LED;
}

// Toggle Red LED
void toggle_red_led() {
  P1OUT ^= RED_LED;
}

// Turn on Green LED
void turn_on_green_led() {
  P1OUT |= GREEN_LED;
}

// Turn off Green LED
void turn_off_green_led() {
  P1OUT &= ~GREEN_LED;
}

// Toggle Green LED
void toggle_green_led() {
  P1OUT ^= GREEN_LED;
}
