//Libraries
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

//define Pins for Data Transmission with Display
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Pin Speaker
int speakerPin = 6;

//Assist var Active Scene
int activeScene;

//func for reading data from Board 1
void receiveEvent(int bytes) {
  //Save active Scene in var that came from board 1
  activeScene = Wire.read();//Receive value from master board
  Serial.print(activeScene);
}
void setup() {
  //Receiving Data
  Wire.begin(9);//9 here is the address(Mentioned even in the master board code) 
  Wire.onReceive(receiveEvent);

  Serial.begin(9600);
  //Setup LCD
  lcd.begin(16, 2);
}

void loop() {
  //Write on LCD ---
  //Line 1
  lcd.setCursor(1,0);
  lcd.print("Active Scene: ");
  lcd.print(activeScene);
  //Line 2
  lcd.setCursor(0,1);
  lcd.print("");
  //---

  //Play sound from speaker connected to arduino. This is NOT used!!! See documention for details
  //Defining what is activated by each scene
  //Scene 1
  if (activeScene == 1) {
    
    tone (speakerPin, 200); //Playback frequency on speaker connected to arduino
  }
  //Scene 2
  else if (activeScene == 2){
    
   tone (speakerPin, 600);   //Playback frequency on speaker connected to arduino
   
  } 
  //Scene 3
  else if (activeScene == 3){
    
    tone (speakerPin, 1200);  //Playback frequency on speaker connected to arduino
   
  } 
  //Scene 4
  else if (activeScene == 4){
    
    tone (speakerPin, 4200);  //Playback frequency on speaker connected to arduino
    
  } 
  //if no Scene is active
  else {
    tone (speakerPin, 8000, 100);  //Playback frequency on speaker connected to arduino, for debuging purposes there is a high pitch
    delay (1000);
  }
}