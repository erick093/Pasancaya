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
char result_V[8];
char result_A[8];
char result_P[8];
char result_E[8];
int maxcounts = 6 ;
//int cont = 0 ;
const int sleepTimeS = 10;

char result[8];

float voltage;
uint8 count[2];
uint8 energy[2];



void setup() {
  Serial.begin ( 115200 );
  //count[1] = 0;
  connectWiFi();
  connectMQTT();
}



void loop() {
  //Serial.println(WiFi.status());
  //Serial.println(client.state());
  delay(500);
  if(WiFi.status() == 6 ){
    Serial.println("Reconnecting WiFi");
    connectWiFi();
  }
  if (!client.connected()){
    Serial.println("Reconnecting MQTT");
    connectMQTT();
  }
  client.loop();
  //Reading Voltave from pin 1 (Multiplexer)
  double read_V=_Voltage(0);
  Serial.print("Sending Voltage: ");
  Serial.println(read_V,5);
  dtostrf(read_V, 6, 5, result_V); //converting FLOAT to CHAR

  if (client.publish(TOPIC_V, result_V)) {
    Serial.println("Publish V ok");
  }
  else {
    Serial.println("Publish V failed");
  }
  if (!client.connected()){
    Serial.println("Reconnecting MQTT");
    connectMQTT();
  }
  //Reading Current from pin 2 (Multiplexer)
  double read_A=_Current(1);
  Serial.print("Sending Amperage: ");
  Serial.println(read_A,5);
  dtostrf(read_A, 6, 5, result_A); //converting FLOAT to CHAR
  client.loop();
  if (client.publish(TOPIC_A, result_A)) {
    Serial.println("Publish A ok");
  }
  else {
    Serial.println("Publish A failed");
  }
  if (!client.connected()){
    Serial.println("Reconnecting MQTT");
    connectMQTT();
  }
  //Obtaining Power
  double read_P=_Power(read_V,read_A);
  Serial.print("Sending Power: ");
  Serial.println(read_P,5);
  dtostrf(read_P, 6, 5, result_P); //converting FLOAT to CHAR
  client.loop();
  if (client.publish(TOPIC_P, result_P)) {
    Serial.println("Publish P ok");
  }
  else {
    Serial.println("Publish P failed");
  }
  //Obtaining Energy
  system_rtc_mem_read(65, count, 2);
  if (count[0] != 123) {
     Serial.println("first time");
     count[0] = 123;
     count[1] = 0;
     energy[1]= 0;
  } else {
     count[1] += 1;
     Serial.print("count = ");
     Serial.println(count[1]);
  }
  system_rtc_mem_write(65, count, 2);
  system_rtc_mem_read(68, energy, 2);
  Serial.print("energy before = ");
  Serial.println(energy[1]);
  energy[1] = energy[1] + (int(read_P)/maxcounts);
  system_rtc_mem_write(68, energy, 2);
  Serial.print("energy after = ");
  Serial.println(energy[1]);
  if (!client.connected()){
    Serial.println("Reconnecting MQTT");
    connectMQTT();
  }
  if (count[1] == maxcounts){
    Serial.print("Sending Energy: ");
    Serial.println(energy[1],5);
    dtostrf(energy[1], 6, 5, result_E); //converting FLOAT to CHAR
    Serial.println(result_E);
    Serial.println("Maxcounts reached");
    client.loop();
    if (client.publish(TOPIC_E, result_E)) {
      Serial.println("Publish E ok");
    }
    else {
      Serial.println("Publish E failed");
    }
    count[1] = 0 ;
    energy[1] = 0;
    system_rtc_mem_write(65, count, 2);
    system_rtc_mem_write(68, energy, 2);
    
  }

  delay(1000);
  Serial.println("ESP8266 in sleep mode");
  system_deep_sleep_set_option(0);
  system_deep_sleep(sleepTimeS * 1000000);
}