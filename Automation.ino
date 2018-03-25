boolean switchOn, lightOn, inLOW, outLOW, automationOn;
int counter, inPIR, outPIR, ldrIn, ldrOut, ldrOutMax, ldrOutMin;

void setup() {
    Serial.begin(9600);
    pinMode(P1_2, INPUT);       //push button for shutdown and on the automation
    pinMode(P3_3, INPUT);       //PIR Sensor Inside
    pinMode(P3_4, INPUT);       //PIR Sensor Outside
    pinMode(P2_4, OUTPUT);      //LED for Outside Sensor Detection
    pinMode(P2_5, OUTPUT);      //LED Inside Sensor Detection
    pinMode(P2_0, OUTPUT);      //Relay for swtich on-off light
    
    digitalWrite(P2_0, LOW);    //Making sure the relay is off
    
    automationOn = true;        //making automation on by default
    switchOn = false;           //making Light off by default
    lightOn = false;            //Current state of light
    inLOW = true;               //Check if the inside PIR sensor is LOW
    outLOW = true;              // check if outside PIR sensor is LOW
    
    inPIR = LOW;                // setting inside PIR sensor state to LOW
    outPIR = LOW;               // Setting outside PIR sensor state to LOW
    
    counter = 0;                //counter for people entered into the room
    ldrIn = 0;                  //inside LDR Reading
    ldrOut = 0;                 //outside LDR Reading
    ldrOutMax = 0;              //variable to store maximum value of outside LDR (for Calibration)
    ldrOutMin = 1023;           //variable to store minimum value of outside LDR (for Calibration)
    
    //calibration of outside LDR starts
    
    digitalWrite(P2_4, HIGH);       //two LED will turn on while calibration starts
    digitalWrite(P2_5, HIGH);
    
    while (millis() < 5000) {
        calibrate();
    }
    
    digitalWrite(P2_4, LOW);        //two LED will turn off when calibration ends (5 seconds)
    digitalWrite(P2_5, LOW);
    
    //calibration of outside LDR ends  
  
}

void loop() {
    
    if(automationOn){
       if(digitalRead(P3_3) == LOW){    //if inside PIR sensor is LOW
           inLOW = true;                //notify inside PIR is LOW
           digitalWrite(P2_5, LOW);     // turn the LED 1 off
       }
       if(digitalRead(P3_4) == LOW){    //if outside PIR sensor is LOW
           outLOW = true;               //notify outside PIR is LOW
           digitalWrite(P2_4, LOW);     // turn the LED 2 off
       }
      
       if(inLOW){                       //if the inside PIR is LOW
           inPIR = digitalRead(P3_3);   // reading the inside PIR state
       }
       if(outLOW){                      //if the outside PIR is LOW
           outPIR = digitalRead(P3_4);  // reading the outside PIR state
       }
       
       
       
       if(outPIR == HIGH && inPIR == LOW){  //if the outside PIR state is HIGH and inside PIR state is LOW
           outPIR = LOW;                    // setting the outside PIR state to LOW
           outLOW = false;                  // but outside PIR is still HIGH for few seconds
           digitalWrite(P2_4, HIGH);        // Turn the LED 2 on
           
           unsigned long startTime = millis();                          //assigning starting time of detecting outside PIR
           unsigned long endTime = startTime;
           while ((endTime - startTime) <= 10000 && inPIR == LOW){      //the loop will run for 10 seconds or until the inside PIR detects the person
               inPIR = digitalRead(P3_3);
               endTime = millis();
           }
           if(inPIR == HIGH){               //if inside PIR detects the person after outside PIR sensor
               digitalWrite(P2_5, HIGH);    //Turn LED 1 on
               inPIR = LOW;                 //setting inside PIR state to LOW
               inLOW = false;               //but inside PIR is still HIGH for few seconds
               
               switchOn = true;             //detected that someone entered into the room so turn the light on
               counter++;                   //increasing the people inside the room
           }
       }else if(inPIR == HIGH && outPIR == LOW){    //if the ontside PIR state is HIGH and outside PIR state is LOW
           inPIR = LOW;                             // setting the inside PIR state to LOW
           inLOW = false;                           // but outside PIR is still HIGH for few seconds
           digitalWrite(P2_5, HIGH);                // Turn the LED 1 on
           
           unsigned long startTime = millis();                        //assigning starting time of detecting inside PIR
           unsigned long endTime = startTime;
           while ((endTime - startTime) <= 10000 && outPIR == LOW){   //the loop will run for 10 seconds or until the outside PIR detects the person
               outPIR = digitalRead(P3_4);
               endTime = millis();
           }
           if(outPIR == HIGH){                      //if outside PIR detects the person after inside PIR sensor
               digitalWrite(P2_4, HIGH);            //Turn LED 1 on
               outPIR = LOW;                        //setting outside PIR state to LOW
               outLOW = false;                      //but outside PIR is still HIGH for few seconds
               
               if(counter > 0){                     //if there are more than zero peope in the room then 
                   counter--;                       //it detects some one just left the room
               }
               if(counter == 0){                    //if there are no people in the room
                   switchOn = false;                // turn the light off
               }
           }
       }
       
       if(switchOn){
           ldrOut = analogRead(A12);        //reading LDR sensors
           ldrIn = analogRead(A4);
           
           ldrOut = map(ldrOut, ldrOutMin, ldrOutMax, 0, 10); //mapping outside LDR reading from 0 to 10
           ldrOut = constrain(ldrOut, 0, 10);
           
           ldrIn = map(ldrIn, ldrOutMin, ldrOutMax, 0, 10);  //mapping inside LDR reading from 0 to 10 according to outside LDR reading
           ldrIn = constrain(ldrIn, 0, 10);
           
           if((ldrIn < 6 && !lightOn) || (ldrOut < 5 && !lightOn)){         //if the outside or inside light is less and the light is not on
               lightOn = true;                                              //then turn the light on
               digitalWrite(P2_0, HIGH);
           }
           if(ldrIn >= 6 && ldrOut >=5){                //if the outside and inside light is more than enough
               lightOn = false;                         // then keep the light off
               digitalWrite(P2_0, LOW);
           }
           
       }else if(!switchOn && lightOn){          //if there is no one in the room and light is on
           digitalWrite(P2_0, LOW);             //then turn off the light
           lightOn = false;
       }
    }
    
    if(digitalRead(P1_2) == HIGH){              //if the push button is pressed
        automationOn = !automationOn;           //toggle the automation state
        
        if(!automationOn){                      // if the automation is off
            digitalWrite(P2_0, LOW);            //turn off the light
            switchOn = false;                   //set the switch to default value
            lightOn = false;                    // set light is not on
            counter = 0;                        // and no people in the room
        }
        delay(1000);
    }
   
}

void calibrate(){               //calibration function of outside LDR
    ldrOut = analogRead(A12);

    if (ldrOut > ldrOutMax) {
      ldrOutMax = ldrOut;
    }

    if (ldrOut < ldrOutMin) {
      ldrOutMin = ldrOut;
    }
    
}
