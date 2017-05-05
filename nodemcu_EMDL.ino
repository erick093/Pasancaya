


#include "wifimqtt.h"
#include "AValueReader.h"
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ThingSpeak.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
extern "C" {
#include "user_interface.h"
}
#define VOLTAGE_MAX 1.0
#define VOLTAGE_MAXCOUNTS 1023.0
LOCAL os_timer_t myTimer;
/* Channel numbers of TS*/
unsigned long V1chan = 241543;
unsigned long A1chan= 254879;

/* Channel numbers of TS*/
const char * myWriteAPIKey = "XZATGLWSQ6FIJA8K"; //API Key of TS
char result_V[8];
char result_A[8];
char result_P[8];
char result_E[8];
bool tickOccured;
float voltage;
int maxcounts = 60 ;
int cont = 0 ;


void timerCallback(void *pArg) {
  tickOccured = true;
}


void user_init(void) {
  //os_timer_disarm(&myTimer);
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 60000, true); //60 seconds interrupt
}

void setup() {
  Serial.begin ( 115200 );
  tickOccured = false;
  connectWiFi();
  connectMQTT();
  ThingSpeak.begin(wificlient);
  user_init();
}



void loop() {
  //Serial.println(WiFi.status());
  //Serial.println(client.state());

  //int sensorValue = analogRead(A0);
  //voltage = sensorValue * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS) ;
  
  //Recconecting Phase
  if(WiFi.status() == 6 ){
    Serial.println("Reconnecting WiFi");
    connectWiFi();
  }
  if (!client.connected()){
    Serial.println("Reconnecting MQTT");
    connectMQTT();
  }


  //Send values to OPENHAB2(MQTT) and TS servers
  //Serial.print("Sending (to MQTT) value: ");
  //Serial.println(voltage,5);
  //dtostrf(voltage , 6, 5, result); //converting FLOAT to CHAR
  // if (client.publish(TOPIC_V, result)) {
  //   Serial.println("Publish ok");
  // }
  // else {
  //   Serial.println("Publish failed");
  // }


  if (tickOccured == true)
  {
    Serial.println("Tick Occurred");
    // Serial.print("Sending(to TS) value: ");
    // Serial.println(voltage,5);
    // ThingSpeak.writeField(V1chan, 1, voltage, myWriteAPIKey);

    //Reading Voltage from pin 1 (Multiplexer)
  	double read_V=_Voltage(1);
  	Serial.print("Sending Voltage: ");
  	Serial.println(read_V,5);
  	dtostrf(read_V, 6, 5, result_V); //converting FLOAT to CHAR
  	//Reading Current from pin 2 (Multiplexer)
  	double read_A=_Current(2);
  	Serial.print("Sending Amperage: ");
  	Serial.println(read_A,5);
  	dtostrf(read_A, 6, 5, result_A); //converting FLOAT to CHAR
  	//Obtaining Power
  	double read_P=_Power(read_V,read_A);
  	Serial.print("Sending Power: ");
  	Serial.println(read_P,5);
  	dtostrf(read_P, 6, 5, result_P); //converting FLOAT to CHAR

  	double energy = energy + (read_P/maxcounts);

    if (client.publish(TOPIC_V, result_V)) {
    	Serial.println("Publish V ok");
  	}
  	else {
    	Serial.println("Publish V failed");
  	}
    if (client.publish(TOPIC_A, result_A)) {
    	Serial.println("Publish A ok");
  	}
  	else {
    	Serial.println("Publish A failed");
  	}
    if (client.publish(TOPIC_P, result_P)) {
    	Serial.println("Publish P ok");
    	cont++;
  	}
  	else {
    	Serial.println("Publish P failed");
  	}

  	if (cont == maxcounts){
	  	dtostrf(energy, 6, 5, result_E); //converting FLOAT to CHAR
  		Serial.println("Maxcounts reached");
	    if (client.publish(TOPIC_E, result_E)) {
    		Serial.println("Publish E ok");
    		cont = 0 ;
    		energy = 0;
		}
  		else {
    		Serial.println("Publish E failed");
  		}

  	}
    tickOccured = false;
  }


  client.loop();
  delay(500);
  yield();
}
