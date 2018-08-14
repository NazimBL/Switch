#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#include <SoftwareSerial.h>
int i;
String id="1";

int stg1[40];
int temp, j, avg;
float power,current;
int sensorValue=200;
bool tag=true;

SoftwareSerial esp(2,3); // RX, TX
bool sendExpect(String sendCMD, String expect,int timeout=400);

int cipstatus();

bool sendHTTP(bool stat = false);

void setup() {
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  Serial.begin(9600);
  while (!Serial); 
  Serial.println("Hola");
  esp.begin(9600);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
   
}

void loop() {
 

  
int s=cipstatus();

Serial.println("CIPSTATUS="+String(s));
if (s==2 || s==1){
  
 digitalWrite(5,1);
 digitalWrite(4,0);
 sendHTTP();
  
}
  else if(s==3){
   sendHTTP(true);
   digitalWrite(5,1);
   digitalWrite(4,0);
  }else if(s==0){
    
    digitalWrite(4,1);
    digitalWrite(5,0);
    sendExpect("AT+RST","OK",5000);
    delay(1000);
    esp.flush();       
    
  }else if(s==5){
    digitalWrite(4,0);
    digitalWrite(5,0);   
    delay(1000);
  }
  else {
    digitalWrite(4,1);
    digitalWrite(5,1);   
    delay(1000);
    
    }
  
  delay(10000);
  
}
bool sendExpect(String sendCMD, String expect, int timeout = 400) {
  esp.flush();
  esp.println(sendCMD);
  String response;
  delay(timeout);
  response = esp.readString();
  Serial.println(response);
  return contains(response, expect);
}

int cipstatus(){
  esp.flush();
  int state;
  String response;
  esp.println("AT+CIPSTATUS");
  delay(100);
  for (i=0 ; i<1000; i++) {
    if (esp.available()>0){
      response=esp.readString();
      break;
      }
    delay(1);
    }
  state=(contains(response,":"))?response.indexOf(":"):0;
  response=response.charAt(state+1);
  return response.toInt();
}

bool contains(String cmd,String resp){

i=0;
int index=cmd.indexOf(resp);

if (index >=0 ) return true;
if (index == -1) return false;
}
bool sendHTTP(bool stat = false) {
  if (!stat){
    if (sendExpect("AT+CIPSTART=\"TCP\",\"178.62.50.10\",80", "OK",400)) {
  
    float data=measure();
    String getcommand = "GET /api/insert.php?id="+id+"&value="+data;  
    
      if (sendExpect("AT+CIPSEND=" + String(getcommand.length()+2), ">")) {
       
       if (sendExpect(getcommand, "SEND",400)) return true;
        
 
       else return false;
       
       } 
    }else Serial.println("TCP CLOSED");
  }else{
   
    float data=measure();
    String getcommand = "GET /api/insert.php?id="+id+"&value="+data;
    
    if (sendExpect("AT+CIPSEND=" + String(getcommand.length()+2), ">")) {
      if (sendExpect(getcommand, "SEND",400))return true;
     
      else return false;
        
    }
  }

   
}

float measure(){
  for (byte i = 0 ; i < 40 ; i++) {
    stg1[i] = 0;
    for (byte j = 0; j < 100; j++) {
      temp = analogRead(A0);
      stg1[i] = (temp > stg1[i]) ? temp : stg1[i];
      delayMicroseconds(100);
    }
  }
  avg = 0;
  for (byte i = 0; i < 40; i++) {
    avg += stg1[i];
  }  
  avg = avg / 40;
 current = 46.08*(float)avg;

 //Serial.println(current);
 power=240.0*current;

 return power*0.001;
}



