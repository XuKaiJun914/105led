#include <EEPROM.h>
//timer
unsigned int timer[4], count1 = 0, count2 = 0, count3 = 0;

//ledfunction
int led[3] = {1, 2, 3};
int clockPin = 4;
int latchPin = 5;
int dataPin = 6;
int dayPlus = 12;
int dayMinus = 13;
int num[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
int day = 200;
int i = 0;


void setup() {

    //timer
    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    TIMSK1 |= (1 << TOIE1);
    TCCR1B |= (1 << CS12);
    sei();
    for (int i = 0; i < 4; i++)
        timer[i] = 0;
    if (EEPROM.read(0) != 255){
      day = 0;
      day += EEPROM.read(0) * 100;
      day += EEPROM.read(1) * 10;
      day += EEPROM.read(2);
    }
}

ISR(TIMER1_OVF_vect) {
    int i;
    TCNT1 = 65224;
    for (i = 0; i < 4; i++) {
        if (timer[i] > 0)
            timer[i]--;
    }

    //ledfunction
    pinMode(1, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(12, INPUT);
    pinMode(13, INPUT);
}


void lb() {
    if(digitalRead(dayPlus) == LOW && digitalRead(dayMinus) == LOW){
      int a = day / 100 % 10;
      int b = day / 10 % 10;
      int c = day % 10;
      EEPROM.write(0, a);
      delay(5);
      EEPROM.write(1, b);
      delay(5);
      EEPROM.write(2, c);
      delay(5);
    }else{
      if (digitalRead(dayPlus) == LOW) {
          day++;
      }
      if (digitalRead(dayMinus) == LOW) {
          day--;
      }
    }
}

void ledf() {
    digitalWrite(led[0], HIGH);
    digitalWrite(led[1], HIGH);
    digitalWrite(led[2], HIGH);
    digitalWrite(led[i], LOW);
    digitalWrite(latchPin, LOW);
    int showNum = 0;
    if (i == 0) showNum = day / 100 % 10;
    if (i == 1) showNum = day / 10 % 10;
    if (i == 2) showNum = day % 10;
    shiftOut(dataPin, clockPin, MSBFIRST, num[showNum]);
    digitalWrite(latchPin, HIGH);
    i++;
    delay(1);
}

void loop() {
    if (timer[0] == 0) { //timer 單位5ms
        timer[0] = 1440000;
        int a = day / 100 % 10;
        int b = day / 10 % 10;
        int c = day % 10;
        EEPROM.write(0, a);
        delay(5);
        EEPROM.write(1, b);
        delay(5);
        EEPROM.write(2, c);
        delay(5);
    }
    ledf();
    if (timer[1] == 0) { //timer 單位5ms
        timer[1] = 20;
        lb();
    }

    if (day >= 999) day = 999;
    if (day <= 0) day = 0;
    if (i >= 3) i = 0;
}
