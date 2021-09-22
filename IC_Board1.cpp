// Libraries needed
#include <Arduino.h>
#include <NewPing.h>
#include <Wire.h>
#include <Servo.h>

//Define PINS for ULTRASONIC
#define TRIGGER_PIN_X 12 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_X 12    // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN_Y 11 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_Y 11    // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

//Create new Objects for Ultrasonic with NewPing Library
NewPing sonar_X(TRIGGER_PIN_X, ECHO_PIN_X, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar_Y(TRIGGER_PIN_Y, ECHO_PIN_Y, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//Assisting Varialbles for Ultrasonic
float duration_X, distance_X, duration_Y, distance_Y;

//FAN ---
// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;

//assistant variables for defining the speed of motors
int motorA, motorB;
//---
Servo servo;

//var for the current active scene
int activeScene;

//spaceMeasurements in cm
int minDistanceFront = 10; // Distance from Sensor to Object min.
int maxDistanceFront = 60; //Distance where front Scene Points
int maxDistanceRear = 160; //Farest distance

void setup()
{
  //Setup for communication with board 2 using Wire Library
  Wire.begin();
  Serial.begin(9600);

  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  servo.attach(6);
}

//func Control of motor speed
void speedControl(int motorA, int motorB)
{
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  //Setting Speed for FANS ---
  // 150 < Speed < 254 !!!!!!!!!!
  analogWrite(enA, motorA);
  analogWrite(enB, motorB);
  //---
}

//func to ALL turn motors off
void TurnMotorsOff()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

//func for EACH Scene

void Scene1() //Nature Summer Day 
{
  speedControl(250, 200);
  //TurnMotorsOff();
}
void Scene2() //City
{
  speedControl(190, 0);
  //TurnMotorsOff();
}
void Scene3() //windy mountaintop
{
  speedControl(250, 250);
  //TurnMotorsOff();
}
void Scene4() //Little River
{
  speedControl(180, 180);
  //TurnMotorsOff();
}

//Calculation of the scene that needs to be set depending on the users position
void setScene(int dist_X, int dist_Y)
{
  //Point A (front Left)
  if (dist_X > minDistanceFront &&
      dist_X < maxDistanceFront &&
      (dist_Y < minDistanceFront ||
       dist_Y > maxDistanceRear))
  {
    //set scene in var
    activeScene = 1;
    //start scene
    Scene1();
  }
  //Point B (front Right)
  if (dist_Y > minDistanceFront &&
      dist_Y < maxDistanceFront &&
      (dist_X < minDistanceFront ||
       dist_X > maxDistanceRear))
  {
    activeScene = 2;
    Scene2();
  }
  //Point C (rear Left)

  if (dist_X > maxDistanceFront &&
      dist_X < maxDistanceRear &&
      (dist_Y < minDistanceFront ||
       dist_Y > maxDistanceRear))
  {
    activeScene = 3;
    Scene3();
  }
  //Point D (rear Right)

  if (dist_Y > maxDistanceFront &&
      dist_Y < maxDistanceRear &&
      (dist_X < minDistanceFront ||
       dist_X > maxDistanceRear))
  {
    activeScene = 4;
    Scene4();
  }

  //For checking if current scene is active in console
  Serial.print("activeScene_");
  Serial.println(activeScene);
}

void loop()
{
  //	speedControl(0,0);

  //ULTRASONIC MEASUREMENT------
  //sonar_X.ping_median();          // takes many duration measurements in a row, throws away the out of range readings and then averages the remaining ones
  //sonar_Y.ping_median();          // takes many duration measurements in a row, throws away the out of range readings and then averages the remaining ones
  distance_X = sonar_X.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)
  distance_Y = sonar_Y.ping_cm(); // Send ping, get distance in cm and print
  setScene(distance_X, distance_Y);

  //Transfering current scene to board 2
  Wire.beginTransmission(9); //9 here is the address of the slave board
  Wire.write(activeScene);
  Wire.endTransmission();
}