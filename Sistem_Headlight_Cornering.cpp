#include <Arduino.h>
#include <Servo.h>
#define POTENTIOMETRU=A0
#define SERVO_STANGA=D3
#define SERVO_DREAPTA=D5
#define LED_STANGA=D6
#define LED_DREAPTA=D7
Servo farStanga;
Servo farDreapta;
const int UNGHIUL_MIN=30;
const int UNGHIUL_MAX=150;
const int UNGHIUL_CENTRU=90;
const unsigned long INTERVAL_CITIRE=50;
const unsigned long INTERVAL_SMOOTH=30;
const float FACTOR_SMOOTH=0.2;
int valoarePot=0;
int unghiTinta=90;
float unghiCurentStanga=90.0;
float unghiCurentDreapta=90.0;
unsigned long ultimaCitire=0;
unsigned long ultimulSmooth=0;
int citesteVolan(){return analogRead(POTENTIOMETRU);}
int calculeazaUnghi(int v){return map(v,0,1023,UNGHIUL_MIN,UNGHIUL_MAX);}
void miscaFaruri(){
unsigned long acum=millis();
if(acum-ultimulSmooth<INTERVAL_SMOOTH)return;
ultimulSmooth=acum;
unghiCurentStanga+=(unghiTinta-unghiCurentStanga)*FACTOR_SMOOTH;
unghiCurentDreapta+=((180-unghiTinta)-unghiCurentDreapta)*FACTOR_SMOOTH;
farStanga.write((int)unghiCurentStanga);
farDreapta.write((int)unghiCurentDreapta);
}
void afiseazaStatus(){
Serial.print("Potentiometru: ");Serial.print(valoarePot);
Serial.print(" | Unghi tinta: ");Serial.print(unghiTinta);
Serial.print(" | Stanga: ");Serial.print(unghiCurentStanga,1);
Serial.print(" | Dreapta: ");Serial.println(unghiCurentDreapta,1);
}
void setup(){
Serial.begin(9600);
pinMode(LED_STANGA,OUTPUT);
pinMode(LED_DREAPTA,OUTPUT);
digitalWrite(LED_STANGA,HIGH);
digitalWrite(LED_DREAPTA,HIGH);
farStanga.attach(SERVO_STANGA);
farDreapta.attach(SERVO_DREAPTA);
farStanga.write(UNGHIUL_CENTRU);
farDreapta.write(UNGHIUL_CENTRU);
}
void loop(){
unsigned long acum=millis();
if(acum-ultimaCitire>=INTERVAL_CITIRE){
ultimaCitire=acum;
valoarePot=citesteVolan();
unghiTinta=calculeazaUnghi(valoarePot);
}
miscaFaruri();
static unsigned long ultimAfisaj=0;
if(acum-ultimAfisaj>=500){
ultimAfisaj=acum;
afiseazaStatus();
}
}
