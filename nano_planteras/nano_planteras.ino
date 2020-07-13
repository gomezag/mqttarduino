#include <RCSwitch.h>
#include <DHT.h>
 #include <LowPower.h>
#define DHTTYPE DHT11
#define DHTPIN 12
RCSwitch mySwitch = RCSwitch();
int sensorbat=A0;
int sensor1=A1;
int sensor2=A2;
int sensor3=A3;
unsigned long data;
int DATALEN=24;
int compPin=10;
int txPin = 3;
int rxPin = 0;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  //  mySwitch.enableReceive(rxPin);
  
    dht.begin();
    pinMode(compPin, OUTPUT);
    digitalWrite(compPin, LOW);
}

void loop() {
    sendComps();
    sendDHT();
    sendBAT();
    for(int i=0;i<38;i++){
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
  }

void sendComps(){
  digitalWrite(compPin, HIGH);
  delay(1000);
  int val1 = analogRead(sensor1);
  int val2 = analogRead(sensor2);
  int val3 = analogRead(sensor3);
  digitalWrite(compPin, LOW);

  float per1 = 100*float(val1)/float(1025);
  float per2 = 100*float(val2)/float(1025);
  float per3 = 100*float(val3)/float(1025);

  data = 3000000+(unsigned long)per1*10000+(unsigned long)per2*100+(unsigned long)per3;
  mySwitch.enableTransmit(txPin);
  mySwitch.setProtocol(1, 350);
  mySwitch.send(data,DATALEN);
}

void sendDHT(){
  delay(1000);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  data = 4000000+(unsigned long)h*100+(unsigned long)t;
  mySwitch.enableTransmit(txPin);
  mySwitch.setProtocol(1, 350);
  mySwitch.send(data,DATALEN);
}

void sendBAT(){
  delay(1000);
  
  int val1 = analogRead(sensorbat);
  float per1 = 100*float(val1)/float(950); //only 950 bc max expected voltage is 4.5V.
  data = 5000000+(unsigned long)per1;
  mySwitch.enableTransmit(txPin);
  mySwitch.setProtocol(1, 350);
  mySwitch.send(data,DATALEN);
}
