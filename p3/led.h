#ifndef LED_H
#define LED_H

// LED Pin Definitions
#define RED_LED BIT0     // Red LED on P1.0
#define GREEN_LED BIT6   // Green LED on P1.6

// Function Prototypes
void init_leds();           // Initialize LED pins
void turn_on_red_led();      // Turn on Red LED
void turn_off_red_led();     // Turn off Red LED
void toggle_red_led();       // Toggle Red LED

void turn_on_green_led();    // Turn on Green LED
void turn_off_green_led();   // Turn off Green LED
void toggle_green_led();     // Toggle Green LED

#endif // LED_H
