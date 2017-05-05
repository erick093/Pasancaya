#ifndef AValueReader_h
#define AValueReader_h
#define AMUXSEL0 14     // MUX Selector 0 D5
#define AMUXSEL1 12     // MUX Selector 1 D6
#define AMUXSEL2 13     // MUX Selector 2 D7
#include "Arduino.h"


void ReadAnalogIn(int chan);
double _Voltage(int _chan);
double _Current(int _chan);
double _Power(double V, double I);


#endif