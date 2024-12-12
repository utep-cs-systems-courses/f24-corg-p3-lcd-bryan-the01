#ifndef STATEMACHINE_H
#define STATEMACHINE_H

void state_advance();
extern volatile short redrawScreen;
extern char blinkFlag;
extern char currentMode;

#endif // STATEMACHINE_H
