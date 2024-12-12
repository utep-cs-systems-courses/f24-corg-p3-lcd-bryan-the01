#ifndef STATEMACHINE_H
#define STATEMACHINE_H

// Allow methods and attributes
// to be accessed by wdt_handler.c
// from watch.c

void state_advance();
extern volatile short redrawScreen;
extern char blinkFlag;
extern char currentMode;

#endif
