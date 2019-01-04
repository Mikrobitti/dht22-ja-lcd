/*
 Original source code for SAA1064 display:
 Example 39.1 - NXP SAA1064 I2C LED Driver IC Demo I
 Demonstrating display of digits
 http://tronixstuff.com/tutorials > chapter 39
 John Boxall July 2011 | CC by-sa-nc
 
 Original source code for DHT22:
 DHT library
 MIT license
 written by Adafruit Industries
 https://github.com/adafruit/DHT-sensor-library/blob/master/DHT.cpp
 
 Combined source code:
 Mikrobitti December 2018, CC by-sa-nc (SAA1064 parts and combined parts) and MIT license (DHT22 parts)
 */
#include "Wire.h" // enable I2C bus
#include "DHT.h"
#define DHTPIN 13     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

byte saa1064 = 0x70 >> 1;
int digits[10]={189, 9, 117, 109, 201, 236, 252, 141, 253, 237};
int lampotilaint;
String lampotilastr;
char numerot[4];

void setup()
{
 Serial.begin(9600); 
 dht.begin();
 Wire.begin();
 pinMode(2, OUTPUT); 
 pinMode(3, OUTPUT); 
 pinMode(4, OUTPUT); 
 pinMode(5, OUTPUT); 
 delay(500);
 initDisplay();
}
void initDisplay()
{
 Wire.beginTransmission(saa1064);
 Wire.write(B00000000); 
 Wire.write(B00010111); 
 clearDisplay();
 Wire.endTransmission();
}
void displayDigits(float lampotila)
{
 lampotilaint=(int)100*lampotila;
 lampotilastr=String(lampotilaint);
 lampotilastr.toCharArray(numerot, 4);
 lampotilastr[0]=int(lampotilastr[0]-48);
 lampotilastr[1]=int(lampotilastr[1]-48);
 lampotilastr[2]=int(lampotilastr[2]-48);
 lampotilastr[3]=int(lampotilastr[3]-48);
 Wire.beginTransmission(saa1064);
 Wire.write(1);
 Wire.write(112); 
 Wire.write(digits[lampotilastr[2]]); 
 Wire.write(digits[lampotilastr[1]]+2);
 Wire.write(+ digits[lampotilastr[0]]);
 Wire.endTransmission();
 delay(10000);
 clearDisplay();
}
void clearDisplay()
{
 Wire.beginTransmission(saa1064);
 Wire.write(1); 
 Wire.write(0);
 Wire.write(0);
 Wire.write(0);
 Wire.write(0);
 Wire.endTransmission();
}
void naytakosteus(float kosteusprosentti) {
 digitalWrite(2, LOW);
 digitalWrite(3, LOW);
 digitalWrite(4, LOW);
 digitalWrite(5, LOW);
 if (kosteusprosentti < 25) {
  digitalWrite(2, HIGH);
 }
 if (kosteusprosentti > 25) {
  digitalWrite(3, HIGH);
 }
 if (kosteusprosentti > 50) {
  digitalWrite(4, HIGH);
 }
 if (kosteusprosentti > 75) {
  digitalWrite(5, HIGH);
 }
}

void loop()
{
 float h = dht.readHumidity();
 float t = dht.readTemperature();
 if (isnan(h) || isnan(t)) {
  Serial.println("Failed to read from DHT sensor!");
  return;
 }
 naytakosteus(h);
 displayDigits(t);
}
