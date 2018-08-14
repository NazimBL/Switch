
int stg1[40];
int temp, i, j;

float power,current,avg;


#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup() {
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  
  cbi(ADMUX, REFS1);
  sbi(ADMUX, REFS0);
  
  Serial.begin(115200);
}

void loop() {
  
  for (i = 0 ; i < 40 ; i++) {
    stg1[i] = 0;
    for (j = 0; j < 100; j++) {
      temp = analogRead(A0);
      stg1[i] = (temp > stg1[i]) ? temp : stg1[i];
      delayMicroseconds(100);
    }
  }
  avg = 0;
  for (i = 0; i < 40; i++) {
    avg += stg1[i];
  }
  
  avg = avg / 40;  
 current = 46.08*avg;
 
 
 //Serial.println(current);
 power=240.0*current;
 //Serial.print(",");
 Serial.println(power*0.001);
 
 /*
 current = 105*(float)avg;
 current/=2;
 //Serial.println(current);
 power=240.0*current;
 //power=33.17*(float)avg;
 //power=25.52*(float)avg;
 Serial.println(power*0.001);
 */
  
   

}


