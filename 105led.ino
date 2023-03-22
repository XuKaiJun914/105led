#include "DHT.h"
#define DHTTYPE DHT11
int led[4] = {2,3,4,5};
int clockPin = 6;
int latchPin = 7;
int dataPin = 8;
int tempPin = 9;
int dayPlus = 0;
int dayMinus = 1;
int num[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
int showTemp = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(dayPlus,INPUT);
  pinMode(dayMinus,INPUT);
  pinMode(led[0],OUTPUT);
  pinMode(led[1],OUTPUT);
  pinMode(led[2],OUTPUT);
  pinMode(led[3],OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  DHT dht(tempPin, DHTTYPE);
}
int day = 200;
int i = 0;
void loop() {
  float h = dht.readHumidity();   //取得濕度
  float t = dht.readTemperature();  //取得溫度C
  digitalWrite(led[0], LOW);
  digitalWrite(led[1], LOW);
  digitalWrite(led[2], LOW);
  digitalWrite(led[3], LOW);
  digitalWrite(led[i], HIGH);
  digitalWrite(latchPin, LOW);
  int showNum = 0;
  if (i % 4 == 0) showNum = day / 1000 % 100;
  if (i % 4 == 1) showNum = day / 100 % 10;
  if (i % 4 == 2) showNum = day / 10 % 10;
  if (i % 4 == 3) showNum = day % 10;
//  showNum = 8;
  shiftOut(dataPin, clockPin, LSBFIRST, num[showNum]);
  digitalWrite(latchPin, HIGH);
  if (digitalRead(dayPlus) == LOW) {
    delay(350);
    day++;
    }
  if (digitalRead(dayMinus) == LOW) {
    delay(350);
    day--;
    }
  i++;
  if(day >= 9999) day = 9999;
  if(day <= 0) day = 0;
  if(i >= 4) i = 0;
  delay(2);
}
