

#include <C:\Users\erick\Documents\Arduino\nodemcu_EMDL/AValueReader.h>

double readAnalogIn(int chan) {
  yield();
  double Ain;
  switch(chan) {
    case 0:
      //Set 8-1 mux to position 0
      digitalWrite(AMUXSEL0, 0);
      digitalWrite(AMUXSEL1, 0);
      digitalWrite(AMUXSEL2, 0);
      delay(100);
     break;
    case 1:
      //Set 8-1 mux to position 1
      digitalWrite(AMUXSEL0, 1);
      digitalWrite(AMUXSEL1, 0);
      digitalWrite(AMUXSEL2, 0);
      delay(100);
      break;
    case 2:
      //Set 8-1 mux to position 2
      digitalWrite(AMUXSEL0, 0);
      digitalWrite(AMUXSEL1, 1);
      digitalWrite(AMUXSEL2, 0);
      delay(100);
      break;
    case 3:
      //Set 8-1 mux to position 3
      digitalWrite(AMUXSEL0, 1);
      digitalWrite(AMUXSEL1, 1);
      digitalWrite(AMUXSEL2, 0);
      delay(100);
      break;
    case 4:
      //Set 8-1 mux to position 4
      digitalWrite(AMUXSEL0, 0);
      digitalWrite(AMUXSEL1, 0);
      digitalWrite(AMUXSEL2, 1);
      delay(100);
      break;
    case 5:
      //Set 8-1 mux to position 5
      digitalWrite(AMUXSEL0, 1);
      digitalWrite(AMUXSEL1, 0);
      digitalWrite(AMUXSEL2, 1);
      delay(100);
      break;
    case 6:
      //Set 8-1 mux to position 6
      digitalWrite(AMUXSEL0, 0);
      digitalWrite(AMUXSEL1, 1);
      digitalWrite(AMUXSEL2, 1);
      delay(100);
      break;
    case 7:
      //Set 8-1 mux to position 7
      digitalWrite(AMUXSEL0, 1);
      digitalWrite(AMUXSEL1, 1);
      digitalWrite(AMUXSEL2, 1);
      delay(100);
      break;
    default:
      break;
  }
  return double(analogRead(A0)); //Read NodeMCU analog input(A0)

  yield();
}
double _Voltage(int _chan){
	double _Vsense;
	_Vsense=readAnalogIn(_chan);
	return abs(_Vsense *0.037236842);
}
double _Current(int _chan){
	//y = 0,05019x - 25,86022 Ecuation of sensor ACS712
	double _Isense;
	_Isense=readAnalogIn(_chan);
	return abs(_Isense * 0.05019 - 25.86022);

}
double _Power(double V, double I){
	return abs(double((V*I)));
}
