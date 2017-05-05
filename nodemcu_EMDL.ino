


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

int maxcounts = 60 ;
int cont = 0 ;
/* Channel numbers of TS*/
const char * myWriteAPIKey = "XZATGLWSQ6FIJA8K"; //API Key of TS
char result[8];
bool tickOccured;
float voltage;



void timerCallback(void *pArg) {
  tickOccured = true;
}


void user_init(void) {
  //os_timer_disarm(&myTimer);
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 15000, true);
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

  int sensorValue = analogRead(A0);
  voltage = sensorValue * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS) ;
  _Voltage(1);
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
  Serial.print("Sending (to MQTT) value: ");
  Serial.println(voltage,5);
  dtostrf(voltage , 6, 5, result); //converting FLOAT to CHAR
  if (client.publish(TOPIC_V, result)) {
    Serial.println("Publish ok");
  }
  else {
    Serial.println("Publish failed");
  }

  if (tickOccured == true)
  {
    Serial.println("Tick Occurred");
    Serial.print("Sending(to TS) value: ");
    Serial.println(voltage,5);
    ThingSpeak.writeField(V1chan, 1, voltage, myWriteAPIKey);
    tickOccured = false;
  }
  client.loop();
  delay(500);
  yield();
}
