
// -------------------------------------------------------------------------
// This project sends Ultrasonic sensor data through Sigfox network.
//
// Created: 22.07.2021
// Author: BeverlyOwino
// --------------------------------------------------------------------------
#include <HCSR04.h>

#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(3,2);
 
#include <String.h>

#define echoPIN 7 
#define trigPIN 8
long duration;
int distance;
String apn =  "ke.celtel.com" ;  
void setup()
{
  gprsSerial.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600);    // the GPRS baud rate 
  pinMode(trigPIN,OUTPUT);
  pinMode(echoPIN,INPUT);
 
  delay(1000);
}
 
void loop()
{
     long duration, inches, cm;
     digitalWrite(trigPIN, LOW);
     delayMicroseconds(2);
     digitalWrite(trigPIN, HIGH);
     delayMicroseconds(10);
     digitalWrite(trigPIN, LOW);
     duration = pulseIn(echoPIN, HIGH);
     distance = duration*(0.034/2);
     //return distance;
     Serial.print(distance); 
      delay(100);       
      
   
  if (gprsSerial.available())
    Serial.write(gprsSerial.read());
 
  gprsSerial.println("AT");
  delay(1000);
 
  gprsSerial.println("AT+CPIN?");
  delay(1000);
 
  gprsSerial.println("AT+CREG?");
  delay(1000);
 
  gprsSerial.println("AT+CGATT?");
  delay(1000);
 
  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
 
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
 
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  //gprsSerial.println("AT+CSTT=\"safaricom\"");//start task and setting the APN,
  gprsSerial.println( "AT+CSTT= \" " + apn + " \" " );//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();

  String str="GET https://api.thingspeak.com/update?api_key=94WXEZ7W5R3UPRLO&field1=" + String(distance);
  //String str="GET https://api.thingspeak.com/update?api_key=O13AOCHYYNU2LQ19&field1=" + String(distance) +"&field2="+String(vin);
  Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  gprsSerial.println();
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 
void ShowSerialData()
{
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000); 
  
}
