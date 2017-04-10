#include "Cage.h"

boolean buttonStateTU = false;
boolean buttonStateTD = false;
boolean buttonStatePU = false;
boolean buttonStatePD = false;
boolean buttonStateST = false;

int WinTemp = 0;

// Set info from .NET elements (called by ArduinoWindow.h)
void portsState(int a, int b, int c, int d) {
	switch(a) {
	case 1:buttonStateTU = 1;
		break;
	case 2:buttonStateTD = 1;
		break;
	case 3:buttonStatePU = 1;
		break;
	case 4:buttonStatePD = 1;
		break;
	case 5:buttonStateST = 1;
		break;
	case 6:WinTemp = 10*b;
	}
	clearSpace(0,0,35,40);
	for(int i=a*5;i;i--) {
		putPixel(i, i);
	}
}

void isr(TIMER1_COMPA_vect) { // timer1_Tick
}

void setup() {
	pinMode(PIN_BuTU, INPUT);
	pinMode(PIN_BuTD, INPUT);
	pinMode(PIN_BuPU, INPUT);
	pinMode(PIN_BuPD, INPUT);
	pinMode(PIN_BuST, INPUT);

	showScreen(0);
	pinMode(PIN_SSR, OUTPUT);
	digitalWrite(PIN_SSR, LOW);
	
	pinMode(PIN_ON, OUTPUT);
	digitalWrite(PIN_ON, HIGH); //ON

	buttonStateST = !digitalRead(PIN_BuST);
	while(buttonStateST) {
		buttonStateST = !digitalRead(PIN_BuST);
	}
}

void neverEnd() {
}
