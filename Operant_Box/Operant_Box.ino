//Operant conditioning Box 2.0
// JPQ & ISH 2021
#include <MsTimer2.h> 

//CODE TO RUN
int BOX_PURGE=0;
int BOX_CALIBRATEVALVE=0; 
int BOX_TRAINING=0;
int BOX_TEST=0;
int BOX_TEST2=1;

//PARAMETERS
const int Start_pin = 10;
int timeBin=100;
int totalTrials=50;
int dropsL=4; int dropsR=5; int caudal=20;
int maxRep=3;
int fixedDelay=15000;//ms for training and test 2
int varDelay=15000;//ms for training and test2
int iniPulse1=10;int iniPulse2=13;int iniPulse3=16;int iniPulse4=19; int iniPulse5=22;//min in which Pulse starts
long pulseDuration=20000; //ms for test1 (20s ON)
long conversion=60000; //ms to minute for test1 (60000ms = 1min)

int left=0;int right=0; int stim=0;
long nextOutput=timeBin-1;
const int ledL=4; const int ledR=5;
const int sensorL=6;const int sensorR=7;
const int valveL=8; const int valveR=9;
int delayVariable=0; int delayReal=0; const int Trigger=11 ; const int sync=10;
int state=1;int currentLed=0; int trialCorrect=0; int trial = 0;
int trialCorrectAccL = 0; int trialCorrectAccR=0;int trialIncorrectAccL = 0; int trialIncorrectAccR=0;int valveCount=0;
int leftResponse=0; int rightResponse=0; int rightResponseAC=0; int leftResponseAC=0;
long ledOnTime=0; long firstChoiceTime=0; long correctChoiceTime=0; long trialIniTime = 0;
int accLedLeft=0; int accLedRight=0;
int TrialNumber=0;
unsigned long startMillis;
int start_done = 0;



void setup() {
  pinMode(Start_pin, INPUT);
  digitalWrite(12,HIGH);
pinMode(ledL,OUTPUT);pinMode(ledR,OUTPUT);pinMode(valveL,OUTPUT);pinMode(valveR,OUTPUT);pinMode(sensorL,INPUT);pinMode(sensorR,INPUT);
randomSeed(analogRead(0));Serial.begin(9600); Serial.flush();
if (BOX_TRAINING==1){
Serial.print("TRIAL"); Serial.print(','); Serial.print("LED"); Serial.print(','); Serial.print("CORRECT");Serial.print(','); Serial.print("CORRECT_AccL");Serial.print(',');
  Serial.print("CORRECT_AccR");Serial.print(',');Serial.print("INCORRECT_AccL");Serial.print(',');Serial.print("INCORRECT_AccR"); Serial.print(',');Serial.print("LED ON (ms)");Serial.print(',');
  Serial.print("FIRST CHOICE (ms)");Serial.print(',');Serial.print("CORRECT CHOICE (ms)"); Serial.println(',');}
  if (BOX_TEST2==1){
Serial.print("TrialNumber"); Serial.print(','); Serial.print("leftResponse"); Serial.print(','); Serial.print("rightResponse");Serial.print(','); Serial.print("leftResponseAC"); Serial.print(','); Serial.print("rightResponseAC");Serial.print(','); 
  Serial.print(',');Serial.print("led on (ms)");Serial.print(',');  Serial.print("Choice time (ms)");Serial.println(',');}
  if(BOX_TEST==1){
   Serial.print("TRIAL"); Serial.print(',');Serial.print("LEFT"); Serial.print(','); Serial.print("RIGHT");Serial.print(','); Serial.print("STIM");Serial.print(','); Serial.print("TIME");Serial.println(',');}

}

void loop() {

if (BOX_PURGE==1){
  if (digitalRead(sensorL)==HIGH){ digitalWrite(valveL,HIGH);delay(caudal);digitalWrite(valveL,LOW);delay(caudal);}
  if (digitalRead(sensorR)==HIGH){ digitalWrite(valveR,HIGH);delay(caudal);digitalWrite(valveR,LOW);delay(caudal);}}


if (BOX_CALIBRATEVALVE==1){
  valveCount=0;
  if (digitalRead(sensorL)==HIGH){ while(valveCount<dropsL){
               digitalWrite(valveL,HIGH);delay(caudal);digitalWrite(valveL,LOW);delay(caudal);valveCount=valveCount + 1;}delay(2000);}
  if (digitalRead(sensorR)==HIGH){ while(valveCount<dropsR){
               digitalWrite(valveR,HIGH);delay(caudal);digitalWrite(valveR,LOW);delay(caudal);valveCount=valveCount + 1;}delay(2000);}}

if (digitalRead(Start_pin)==HIGH){
  if (start_done==0){
    startMillis = millis();
    start_done = 1;
  }
  if(BOX_TRAINING==1){
   if (state ==1){

      trialIniTime=millis();
      delay(fixedDelay); 

      currentLed=random(1,3); 
      if (currentLed==1){accLedLeft=accLedLeft+1; accLedRight=0;}
      if (currentLed==2){accLedRight=accLedRight+1; accLedLeft=0;}
      if (accLedLeft==maxRep+1){currentLed=2;accLedRight=1; accLedLeft=0;}
      if (accLedRight==maxRep+1){currentLed=1;accLedLeft=1; accLedRight=0;}

      delay(random(1,varDelay));

       if(currentLed==1){//left
         digitalWrite(ledL,HIGH);ledOnTime=millis();state=2;}
       if(currentLed==2){//right
         digitalWrite(ledR, HIGH);ledOnTime=millis();state=2;}}
   if (state==2){

      if (digitalRead(sensorL)==HIGH){//bicho en sensor izquierdo
          if(currentLed==1){
            trialCorrect=1;trialCorrectAccL=trialCorrectAccL + 1;
            firstChoiceTime=millis();valveCount=0;
            while(valveCount<dropsL){
                 digitalWrite(valveL,HIGH);delay(caudal);digitalWrite(valveL,LOW);delay(caudal);valveCount=valveCount + 1;}state=4;}
           if(currentLed==2){
            trialCorrect=0;trialIncorrectAccL=trialIncorrectAccL + 1;firstChoiceTime=millis();state=3;}}//MODIFICADO AQUÍ!!! STATE=3 DENTRO DEL LOOP DE IF(CURRENTLED==2)


       if (digitalRead(sensorR) ==HIGH){//bicho en el sensor derecho
          if(currentLed==2){
            trialCorrect=1;trialCorrectAccR=trialCorrectAccR + 1;
            firstChoiceTime=millis();valveCount=0;
            while(valveCount<dropsR){
                 digitalWrite(valveR,HIGH);delay(caudal);digitalWrite(valveR,LOW);delay(caudal);valveCount=valveCount + 1;}state=4;}
           if(currentLed==1){
            trialCorrect=0;trialIncorrectAccR=trialIncorrectAccR + 1;firstChoiceTime=millis();state=3;}}}//MODIFICADO AQUÍ! STATE=3 DENTRO DEL LOOP DE IF(CURRENTLED==2)

    if (state==3){
          if (digitalRead(sensorL)==HIGH && currentLed==1){//bicho en sensor izquierdo
            valveCount=0;
            while(valveCount<dropsL){
                 digitalWrite(valveL,HIGH);delay(caudal);digitalWrite(valveL,LOW);delay(caudal);valveCount=valveCount + 1;correctChoiceTime=millis();}state=4;}

       if (digitalRead(sensorR)==HIGH && currentLed==2){//bicho en el sensor derecho
            valveCount=0;
            while(valveCount<dropsR){
                 digitalWrite(valveR,HIGH);delay(caudal);digitalWrite(valveR,LOW);delay(caudal);valveCount=valveCount + 1;correctChoiceTime=millis();}state=4;}}

    if (state==4){
      digitalWrite(ledL,LOW);digitalWrite(ledR,LOW);state=1;if(TrialNumber==totalTrials-1){BOX_TRAINING=0;}

    TrialNumber=TrialNumber + 1;
    if (trialCorrect==1){correctChoiceTime=firstChoiceTime;};
    if(TrialNumber>0){
    Serial.print(TrialNumber); Serial.print(','); Serial.print(currentLed); Serial.print(','); Serial.print(trialCorrect);Serial.print(','); Serial.print(trialCorrectAccL);Serial.print(',');
    Serial.print(trialCorrectAccR);Serial.print(',');Serial.print(trialIncorrectAccL);Serial.print(',');Serial.print(trialIncorrectAccR); Serial.print(',');Serial.print(ledOnTime-trialIniTime);Serial.print(',');
    Serial.print(firstChoiceTime-ledOnTime);Serial.print(',');Serial.print(correctChoiceTime-ledOnTime); Serial.println(',');}
      }

  }

  if (BOX_TEST==1) {
   if((nextOutput==(iniPulse1*conversion-1))||(nextOutput==(iniPulse2*conversion-1))||(nextOutput==(iniPulse3*conversion-1))||(nextOutput==(iniPulse4*conversion-1))||(nextOutput==(iniPulse5*conversion-1))){
    digitalWrite(Trigger,HIGH);
      if (stim==0){trial=trial+1;} stim=1;}
  if((nextOutput==((iniPulse1*conversion)+pulseDuration-1))||(nextOutput==((iniPulse2*conversion)+pulseDuration-1))||(nextOutput==((iniPulse3*conversion)+pulseDuration-1))||(nextOutput==((iniPulse4*conversion)+pulseDuration-1))||(nextOutput==((iniPulse5*conversion)+pulseDuration-1))){
    digitalWrite(Trigger,LOW);stim=0;}

    if (digitalRead(sensorL)==HIGH){left=1;};
    if (digitalRead(sensorR)==HIGH){right=1;};

    if (nextOutput<millis()){
    Serial.print(trial); Serial.print(',');Serial.print(left); Serial.print(','); Serial.print(right);Serial.print(','); Serial.print(stim);Serial.print(','); Serial.print(millis()-startMillis);Serial.println(',');
    digitalWrite(sync,HIGH); delay(10);digitalWrite(sync,LOW);nextOutput=nextOutput+timeBin;left=0;right=0;};
  }

  if(BOX_TEST2==1){
   if (state ==1){
     trialIniTime=millis();leftResponse=0; rightResponse=0;
      delay(fixedDelay); delay(random(1,varDelay));
      digitalWrite(ledL,HIGH);digitalWrite(ledR, HIGH);ledOnTime=millis();state=2;}

   if (state==2){
      if (digitalRead(sensorL)==HIGH){//bicho en sensor izquierd
            firstChoiceTime=millis();leftResponse=1; leftResponseAC=leftResponseAC+1;
            valveCount=0;digitalWrite(ledL,LOW);digitalWrite(ledR,LOW);//MODIFICADO AQUÍ. LEDS SE APAGAN ANTES DE ACTIVAR VÁLVULA.
            while(valveCount<dropsL){
                 digitalWrite(valveL,HIGH);delay(caudal);digitalWrite(valveL,LOW);delay(caudal);valveCount=valveCount + 1;}state=3;}


       if (digitalRead(sensorR) ==HIGH){//bicho en el sensor derecho

            firstChoiceTime=millis();rightResponse=1; rightResponseAC=rightResponseAC+1;
            valveCount=0;digitalWrite(ledL,LOW);digitalWrite(ledR,LOW);//MODIFICADO AQUÍ. LEDS SE APAGAN ANTES DE ACTIVAR VÁLVULA.
            while(valveCount<dropsR){
                 digitalWrite(valveR,HIGH);delay(caudal);digitalWrite(valveR,LOW);delay(caudal);valveCount=valveCount + 1;}state=3;}}

    if (state==3){
      state=1;if(TrialNumber==totalTrials-1){BOX_TEST2=0;}
    TrialNumber=TrialNumber + 1;
    if(TrialNumber>0){
    Serial.print(TrialNumber); Serial.print(','); Serial.print(leftResponse); Serial.print(','); Serial.print(rightResponse);Serial.print(','); Serial.print(leftResponseAC); Serial.print(','); Serial.print(rightResponseAC); 
    Serial.print(',');Serial.print(ledOnTime-trialIniTime);Serial.print(',');  Serial.print(firstChoiceTime-ledOnTime);Serial.println(',');}
      }
    }
}
}