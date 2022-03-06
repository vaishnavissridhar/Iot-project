#include <SoftwareSerial.h>
#include "DHT.h"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);

 #define DHTPIN 7        // Digital pin connected to the DHT sensor
 #define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial mySerial(9, 10);

float soil=0;
float light=0;
int h;
int t;

void setup()
{
  
  lcd.init();
  dht.begin();
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);     // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);

  pinMode(7, INPUT);      // DHT
  pinMode(A1, INPUT);     // soil moisture
  pinMode(A6, INPUT);     // LDR
  pinMode(5, OUTPUT);     // Relay
  pinMode(3, OUTPUT);     // LED
  
  digitalWrite(5,HIGH);   // Relay pin HIGH
}

void loop()
{
 delay(2000);
 digitalWrite(5,HIGH);
 soil=analogRead(A1);
 light=analogRead(A6);
 h = dht.readHumidity();
 t = dht.readTemperature();
      
      lcd.backlight();
      lcd.setCursor(1,0);
      lcd.print("Temperature ");
      lcd.setCursor(13,0);
      lcd.print(t);
      lcd.setCursor(1,1);
      lcd.print("Humidity  ");
      lcd.setCursor(10,1);
      lcd.print(h);

if(light<300)
{
  digitalWrite(3,HIGH);
}
if(light>300)
{
  digitalWrite(3,LOW);
}
      
 if(soil>600)
 {
  digitalWrite(5,LOW);
  delay(500);
  digitalWrite(5,HIGH);
 }
 Serial.print("Soil");
 Serial.println(soil);
 Serial.print("Light");
 Serial.println(light);
 Serial.print("Temperature");
 Serial.println(t);
 Serial.print("Humdity");
 Serial.println(h);
 
  SendMessage();    // function to send sms
      
 if (mySerial.available()>0)
   Serial.write(mySerial.read());
}

void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+919778385673\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.print("Soil Moisture: ");// The SMS text you want to send
  mySerial.println(soil);// The SMS text you want to send
  mySerial.print("Humidity: ");
  mySerial.println(h);
  mySerial.print("Temperature: ");
  mySerial.println(t);
  mySerial.print("Light Intensity: ");
  mySerial.println(light);
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(10000);
}
