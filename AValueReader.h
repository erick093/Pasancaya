#ifndef AValueReader_h
#define AValueReader_h
#define AMUXSEL0 5     // MUX Selector 0 D1
#define AMUXSEL1 12     // MUX Selector 1 D2
#define AMUXSEL2 13     // MUX Selector 2 D7
#include "Arduino.h"
#include <SoftwareSerial.h>


void ReadAnalogIn(int chan);
double _Voltage(int _chan);
double _Current(int _chan);
double _Power(double V, double I);


#endif