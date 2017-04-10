#include "Display.h"

void setup();
extern void isr();
extern void portsState(int a,int b,int c,int d);

#define digitalRead(a) true
#define digitalWrite(a,b) true
#define analogRead(a) 0
#define pinMode(a,b) ;
#define delay(a) ;
#define TIMER1_COMPA_vect

void neverEnd();