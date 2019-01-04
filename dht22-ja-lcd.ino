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

byte saa1064 = 0x70 >> 1; // define the I2C bus address for our SAA1064 (pin 1 to GND) ****
int digits[10]={189, 9, 117, 109, 201, 236, 252, 141, 253, 237};
// these are the byte representations of pins required to display each digit 0~9 
int lampotilaint;
String lampotilastr;
char numerot[4];

void setup()
{
 Serial.begin(9600); 
 Serial.println("Aloitetaan.");
 dht.begin();
 Wire.begin(); // start up I2C bus
 pinMode(2, OUTPUT); 
 pinMode(3, OUTPUT); 
 pinMode(4, OUTPUT); 
 pinMode(5, OUTPUT); 
 delay(500);
 initDisplay();
}
void initDisplay()
// turns on dynamic mode and adjusts segment current to 12mA
{
 Wire.beginTransmission(saa1064);
 Wire.write(B00000000); // this is the instruction byte. Zero means the next byte is the control byte
 //Wire.write(B01010011); // control byte (dynamic mode on, digits 1+3 on, digits 2+4 on, 12mA segment current
  Wire.write(B00010111); // control byte (3ma in this case)
 clearDisplay();
 Wire.endTransmission();
}
void displayDigits(float lampotila)
{
 Serial.print("lampotila: ");
 Serial.println(lampotila);
 lampotilaint=(int)100*lampotila;
 lampotilastr=String((int)lampotilaint);
 lampotilastr.toCharArray(numerot, 4);
 lampotilastr[0]=int(lampotilastr[0]-48);
 lampotilastr[1]=int(lampotilastr[1]-48);
 lampotilastr[2]=int(lampotilastr[2]-48);
 lampotilastr[3]=int(lampotilastr[3]-48);
 Wire.beginTransmission(saa1064);
 Wire.write(1); // instruction byte - first digit to control is 1 (right hand side)
 // -48 että saadaan char-asciista int
 Wire.write(112); // c //[lampotilastr[3]]); // digit 1 (RHS)
 Wire.write(digits[lampotilastr[2]]); // digit 2
 // +2 = 01000000 = desimaalipiste
 Wire.write(digits[(int)lampotilastr[1]]+2); // digit 3
 Wire.write(+ digits[lampotilastr[0]]); // digit 4 (LHS)
 Wire.endTransmission();
 Serial.println(digits[lampotilastr[3]]); // digit 1 (RHS)
 Serial.println(digits[lampotilastr[2]]); // digit 2
 Serial.println(digits[lampotilastr[1]]); // digit 3
 Serial.println(digits[lampotilastr[0]]); // digit 4 (LHS)
 delay(10000);
 clearDisplay();
}
void clearDisplay()
// clears all digits
{
 Wire.beginTransmission(saa1064);
 Wire.write(1); // instruction byte - first digit to control is 1 (right hand side)
 Wire.write(0); // digit 1 (RHS)
 Wire.write(0); // digit 2
 Wire.write(0); // digit 3
 Wire.write(0); // digit 4 (LHS)
 Wire.endTransmission();
}
void naytakosteus(float kosteusprosentti) {
 digitalWrite(2, LOW);
 digitalWrite(3, LOW);
 digitalWrite(4, LOW);
 digitalWrite(5, LOW);
 if (kosteusprosentti < 25) {
  Serial.println("Alle 25: vihrea");
  digitalWrite(2, HIGH);
 }
 if (kosteusprosentti > 25) {
  Serial.println("Yli 25: keltainen");
  digitalWrite(3, HIGH);
 }
 if (kosteusprosentti > 50) {
  Serial.println("Yli 50: punainen");
  digitalWrite(4, HIGH);
 }
 if (kosteusprosentti > 75) {
  digitalWrite(5, HIGH);
  Serial.println("Yli 75: valkoinen");
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
 Serial.print("Kosteusprosentti: "); 
 Serial.print(h);
 Serial.println(" %");
 Serial.print("Lampotila: "); 
 Serial.print(t);
 Serial.println(" *C");
 naytakosteus(h);
 displayDigits(t);
}
/* **** We bitshift the address as the SAA1064 doesn't have the address 0x70 (ADR pin
to GND) but 0x38 and Arduino uses 7-bit addresses- so 8-bit addresses have to
be shifted to the right one bit. Thanks to Malcolm Cox */
