#include <amt1001_ino.h>
#include <RCSwitch.h>
#include <string.h>
#include <MicroView.h>
#include <TimeAlarms.h>
#include <Time.h>
// #include "face.h"
// #include "bitmaps.h"
#include "rfhandler.h"
#define DATALEN 24
#define AFTER_RF_SLEEP 1000
#define RELAY_DELAY 500
#define FACE_DELAY 500
#define MAX_TIME_ON 35 // seconds

/*
states for this device:
0: idle
1: up
2: down
3: shut_down
4: send_temp

*/

unsigned long dictionary[5] = {10001111, 9392466, 9392472, 11100000, 10000001};
RFhandler rfhandler=RFhandler(dictionary, 5);

const int txPin=3;
const int rxPin=0;
const int pinLen=4;
const int pins[pinLen]={9,10,11,12};
unsigned long data;

//Face face=Face();
RCSwitch mySwitch = RCSwitch();

const byte numChars=4;
char receivedChars[numChars];
boolean newData = false;

int incomingByte=0;
int powerPin = 0;
int dirPin = 1;
int tempPin = A0;
int humPin = A1;

char current_msg[30] = "Hello";

int stopAlarm;

void setup() 
  {
   // Serial.begin(115200);
    setTime(8,29,0,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011
    mySwitch.enableReceive(0);
    pinMode(powerPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(tempPin, INPUT);
    pinMode(humPin, INPUT);
    digitalWrite(powerPin, LOW);
    digitalWrite(dirPin, LOW);

//     uView.begin();
//     uView.clear(PAGE);
//     face.change_face(smile);
//     face.viewPrint("Hi!");
//     uView.display();
    send();
  }
 
void loop() 
{
  if(mySwitch.available()) {
//    face.clear_lnk();
//    face.add_bitmaps(dwnlnk);
//    face.change_face(surprise);
//    uView.display();
    unsigned long value = mySwitch.getReceivedValue();
    int prev_state = rfhandler.state();
    int new_state = rfhandler.parse_state(value);
    if(prev_state==1 || prev_state==2){
        if(prev_state==new_state){
            new_state = rfhandler.parse_state(dictionary[3]);
        }
        else{
            if(new_state==4 || new_state==0){
                new_state = prev_state;
            }
        }
    }
    switch(new_state) {
        case 0:
            idle();
            break;
        case 1:
            Alarm.timerOnce(MAX_TIME_ON, stop);
            goUp();
            break;
        case 2:
            Alarm.timerOnce(MAX_TIME_ON, stop);
            goDown();
            break;
        case 3:
            stop();
            break;
        case 4:
            send();
            break;
        default:
//            face.change_face(hmm);
//            uView.display();
            delay(FACE_DELAY);
            rfhandler.set_idle();
            idle();
            break;
    }
//    face.clear_lnk();
//    face.viewPrint(current_msg);
//    Serial.println(current_msg);
//    uView.display();
    delay(AFTER_RF_SLEEP);
    mySwitch.resetAvailable();
  }
  Alarm.delay(3);
}

void goUp() {
//    face.change_face(working);
    digitalWrite(dirPin, HIGH);
    delay(RELAY_DELAY);
    digitalWrite(powerPin, HIGH);
//    face.viewPrint("Going up");
}

void goDown() {
//    face.change_face(working);
    digitalWrite(dirPin, LOW);
    delay(RELAY_DELAY);
    digitalWrite(powerPin, HIGH);
//    face.viewPrint("Going down");
}

void stop() {
//    face.change_face(working);
    digitalWrite(powerPin, LOW);
    delay(RELAY_DELAY);
    digitalWrite(dirPin, LOW);
//    face.change_face(smile);
    rfhandler.set_idle();
    idle();
}

void send() {
//    face.change_face(smile);
    sendData(&current_msg);
    rfhandler.set_idle();
    idle();
}

void idle() {
//    face.change_face(smile);
//    uView.display();
}

void sendData(char (*current_msg)[30]){
  uint16_t step = analogRead(tempPin);
  uint16_t temp = amt1001_gettemperature(step);
  step = analogRead(humPin);
  double volt = (double)step * (5.0/1023.0);
  uint16_t hum = amt1001_gethumidity(volt);
  char ctemp[5];
  char chum[5];
//  face.clear_lnk();
//  face.add_bitmaps(uplnk);
//  uView.display();
  Alarm.delay(1000);
  mySwitch.enableTransmit(txPin);
  mySwitch.setProtocol(1, 350);
  dtostrf(temp, 0, 0, ctemp);
  dtostrf(hum, 0, 0, chum);
  sprintf(*current_msg, "T%s%cC H%s%c", ctemp, 167, chum, 37); //167 is the degree symbol and 37 the percentage symbol
  data = temp*1000+hum+2*1000000;
  delay(500);
  mySwitch.send(data, DATALEN);
  mySwitch.enableReceive(rxPin);
//  uView.display();
}

void lookAck(){
//    face.change_face(looking);
//    face.viewPrint("...");
//    face.clear_lnk();
//    face.add_bitmaps(uplnk);
//    uView.display();
    mySwitch.enableTransmit(txPin);
    mySwitch.setRepeatTransmit(10);
    mySwitch.setProtocol(1, 350);
    data = 11110000;
    mySwitch.send(data, DATALEN);
    mySwitch.enableReceive(rxPin);
//    face.change_face(smile);
//    uView.display();
}
