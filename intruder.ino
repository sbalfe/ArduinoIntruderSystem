#include "LiquidCrystal.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(1, 11, 5, 4, 7, 6);

int LDR = A1;
int toggleAlarm = 2;
int piezo = 12;
int triggerPin = 8;
int echoPin = 13;
int distanceThresholdChange = 3;
int red = 10;
int green = 9;
int distanceThreshold = 20;
int cm = 0;
int inches = 0;
float sinVal;
int toneVal;
float distance = 4000;
int index =0;
int lightPercentage;

bool toggleLights = false;
bool messageON = false;
bool messageOFF = false;
bool alarm = 0;
bool flashGreen = false;
bool disarm = false;

char onMessage[] = "Lights On";
char offMessage[] = "Lights Off";

void setup() {
  pinMode(LDR , INPUT);
  pinMode(toggleAlarm, INPUT_PULLUP);
  pinMode(piezo, OUTPUT);
  
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(distanceThresholdChange, INPUT_PULLUP);
  
  lcd.begin(16,2);
 
  attachInterrupt(digitalPinToInterrupt(2),toggleAlarmSwitch, FALLING );
  attachInterrupt(digitalPinToInterrupt(3), shiftDistanceThreshold, FALLING);
  
}

// This function uses the time to travel to a certain object
// in terms of the sound wave. 
long readDistanceSensor(int triggerPin, int echoPin){
  
  // clear the trigger pin by writing low to it
  pinMode(triggerPin, OUTPUT); 
  digitalWrite(triggerPin, LOW);
  
  // wait 2 microseconds
  delayMicroseconds(2);
  
  // Sets the trigger pin to HIGH for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  
  //reads the echo pin, and returns the sound wave travel time in microseconds
  
  return pulseIn(echoPin, HIGH);
  
}

void checkAlarm(float distance){
  
  
    // If the intruder is within the threshold to set the alarm off
    if (distance < distanceThreshold){
    
      // as long as the alarm is playing 
      while(alarm){
        // display INTRUDER on the screen
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("INTRUDER!");
   
        // Once the alarm is disarmed it should flash green once
        // therefore every time its set off reset this boolean
        flashGreen = true;
        disarm = true; // control the message so it displays once
        
        //turn on piezo alarm
        alarmNoise();
        
        //if light is low enough for this boolean to toggle then 
        // turn on the red light
        if(toggleLights){
          digitalWrite(red,HIGH); 
   
        }
      }
    
      // turn off piezo when alarm is disarmed
      noTone(piezo);
      
      // flash green if lights toggle is on and it hasnt done so before
      if (flashGreen && toggleLights){
     
        // turn off red light
        digitalWrite(red,LOW);
        
        //turn on green light and wait a 1000ms then turn back off.
        digitalWrite(green,HIGH);
        delay(1000);
        digitalWrite(green,LOW);
        
      }
      if(disarm){
        lcd.clear();
        lcd.setCursor(0,0);
        //display alarm disarmed on screen
        lcd.print("Alarm Disarmed");
      }  
 
    }
  
    // dont flash green again
    flashGreen = false;
    disarm = false;
}

void loop() {
  
  distance = 0.01723 * readDistanceSensor(triggerPin, echoPin);
  //Serial.print(distance);
  //Serial.print("cm\n");
  
  checkAlarm(distance);
  checkLightValue();
  updateDisplay();
  
}

void alarmNoise() {
  
  for (int x=0; x<180; x++) {
      
        // stop this loop to ensure the alarm doesnt continue when
        // the user has it disarmed
      if(!alarm){
        break;
      }
    // convert degrees to radians then obtain sin value
    sinVal = (sin(x*(3.1412/180)));
    // generate a frequency from the sin value
    toneVal = 2000+(int(sinVal*1000));
    tone(piezo, toneVal);
    delay(2);
  }
}

// switch the alarm boolean on and off
void toggleAlarmSwitch(){
  
  
  (alarm)?alarm=0:alarm=1;
  lcd.clear(); // clear it as it changes the active to inactive
}

void shiftDistanceThreshold(){
  
  // if the distance threshold is 300 then reset back to 20 otherwise increment by 20.
  (distanceThreshold%300==0)?distanceThreshold=20:distanceThreshold+=20; 
}

void updateDisplay(){
  char topLine[128]=" status: ";
  // Use this string to concatenate , used 128 spaces to ensure there is always enough space
  char bottomLine[128] = "Lights: ";
  
  // write on first line and first column
  lcd.setCursor(0,0);
  
  // the status of the alarm, whether its on or off.
 
  (alarm)? strcat(topLine, "active |"):strcat(topLine,"inactive |");
  
  // view light percentage converting int to char[] as same as below
  char lightValue[5];
  
  strcat(topLine," Light Percentage: ");
  strcat(topLine, itoa(lightPercentage, lightValue, 10));
  strcat(topLine,"% |");
  delay(200);
  
  //Scroll topLine as there is not enough space
  ScrollLine(topLine);
  // write on the second line first column
  lcd.setCursor(0,1);
  
  // whether the lights are on or off
  (toggleLights)? strcat(bottomLine,"On "):strcat(bottomLine,"Off ");
  
  // create a new char array to convert distance threshold to a string
  char distance[5];
  
  // concatenate all the data together calling itoa to convert distance threshold to a char array
  strcat(bottomLine,"| DistanceToActivate: ");
  strcat(bottomLine,itoa(distanceThreshold, distance,10));
  strcat(bottomLine,"cm | ");
  
  // Scroll the bottomLine as the data does not entirely fit on the screen.
  ScrollLine(bottomLine);   
}

bool displayShortMessage(bool toggle, char message[]){
 
  // if the message has not been displayed yet, display it and wait 1000ms before wiping
  if (!toggle){
      
        lcd.setCursor(0,0);
      lcd.clear();
      lcd.print(message);
      delay(1000);
      lcd.clear();
           
  }
  // toggle is true therefore return true again to make sure next call it wipes the screen
  return true;   
  
}

void checkLightValue(){
  
  // read the light resistor value
  int readRes = analogRead(LDR);
  
  //convert it into 1-100 range. 
  lightPercentage = map(readRes,1,310,1,100);
  
  // if light is below a certain value
  if (lightPercentage < 5){
    
    //turn lights off
    toggleLights = true;
    
    //theOFF message is no longer on as to say it can be displayed again now
    messageOFF = false;
    
    //display the message setting the ON to true to say to only display it once. 
    messageON = displayShortMessage(messageON, onMessage);
      
  }
  
  else{
    // same as above but for turning off lights and displaying off message
    toggleLights = false;
    messageON = false;
    messageOFF = displayShortMessage(messageOFF, offMessage);
   
  }
}



void ScrollLine(char message[]){
 
 
  // increment the index value to show where to start printing characters from 
  index++;
  
  // get the length of the message used in taking the mod so it can wrap around
  int length = strlen(message);
  
  // go through and fetch 16 characters to add to lcd
  for(int i = 0; i < 16; i++){
    
    // offset must be shifted along the line as to make it seem the word is being scrolled along
    int offset = i+index;
    
    // index with the offset and mod to wrap round when it finishes the message
    char letterToAdd = message[offset%length];
    
    // write the character to the lcd.
    lcd.write(letterToAdd);
    
    
  }
  
}
