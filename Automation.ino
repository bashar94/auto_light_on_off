boolean switchOn;
boolean lightOn;
int counter;
int inPIR;
int outPIR;

void setup() {
    Serial.begin(9600);
    
  pinMode(P3_3, INPUT);   //PIR Sensor In
  pinMode(P3_4, INPUT);   //PIR Sensor Out
  
  pinMode(P2_4, OUTPUT);   //LED
  pinMode(P2_5, OUTPUT);   //LES
  
  switchOn = false;
  lightOn = false;
  
  inPIR = LOW;
  outPIR = LOW;
  
  counter = 0;
  
}

void loop() {
  
   
   if(lightOn){
       inPIR = digitalRead(P3_3); 
   }else{
       outPIR = digitalRead(P3_4);
   }
   
   
   
   if(outPIR == HIGH && inPIR == LOW){
       outPIR = LOW;
       digitalWrite(P2_4, HIGH);
       digitalWrite(P1_5, LOW);
       Serial.println("out High");
       unsigned long startTime = millis();
       unsigned long endTime = startTime;
       while ((endTime - startTime) <= 10000 && inPIR == LOW){
           inPIR = digitalRead(P3_3);
           endTime = millis();
       }
       if(inPIR == HIGH){
           digitalWrite(P2_5, HIGH);
           digitalWrite(P2_4, LOW);
           inPIR = LOW;
           Serial.println("in High");
           switchOn = true;
           counter++;
           Serial.println(counter);
       }
   }else if(inPIR == HIGH && outPIR == LOW){
       inPIR = LOW;
       digitalWrite(P2_5, HIGH);
       digitalWrite(P2_4, LOW);
       Serial.println("in High");
       unsigned long startTime = millis();
       unsigned long endTime = startTime;
       while ((endTime - startTime) <= 10000 && outPIR == LOW){
           outPIR = digitalRead(P3_4);
           endTime = millis();
       }
       if(outPIR == HIGH){
           digitalWrite(P2_4, HIGH);
           digitalWrite(P2_5, LOW);
           outPIR = LOW;
           Serial.println("out High");
           if(counter > 0){
               counter--;
           }
           Serial.println(counter);
           if(counter == 0){
               switchOn = false;   
           }
       }
   }
   
   if(switchOn && !lightOn){
       Serial.println("Light On");
       lightOn = true;
       
   }else if(!switchOn && lightOn){
       Serial.println("Light Off");
       lightOn = false;
   }
   
}
